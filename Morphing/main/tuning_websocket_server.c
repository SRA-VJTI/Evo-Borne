#include "include/tuning_websocket_server.h"

static const char *TAG = "websocket_server";

static comms_val_t comms_val = { .test = 1.0, .speed = 0, .right_turn = false, .left_turn = false, .front = false, .back = false, .val_changed = false };

static QueueHandle_t client_queue;
const static int client_queue_size = 10;

static void initialise_mdns(void)
{
    mdns_init();
    mdns_hostname_set(MDNS_HOST_NAME);
    mdns_instance_name_set(MDNS_INSTANCE);

    mdns_txt_item_t serviceTxtData[] = {
        {"board", "esp32"},
        {"path", "/"}};

    ESP_ERROR_CHECK(mdns_service_add("ESP32-WebServer", "_http", "_tcp", 80, serviceTxtData,
                                     sizeof(serviceTxtData) / sizeof(serviceTxtData[0])));
}

void websocket_callback(uint8_t num, WEBSOCKET_TYPE_t type, char *msg, uint64_t len)
{
    switch (type)
    {
    case WEBSOCKET_CONNECT:
        ESP_LOGI(TAG, "client %d connected!", num);
        break;
    case WEBSOCKET_DISCONNECT_EXTERNAL:
        ESP_LOGI(TAG, "client %d sent a disconnect message", num);
        break;
    case WEBSOCKET_DISCONNECT_INTERNAL:
        ESP_LOGI(TAG, "client %d was disconnected", num);
        break;
    case WEBSOCKET_DISCONNECT_ERROR:
        ESP_LOGI(TAG, "client %d was disconnected due to an error", num);
        break;
    case WEBSOCKET_TEXT:
        if (len)
        { // if the message length was greater than zero
            comms_val.val_changed = true;

            switch (msg[0])
            {
                case 'T':
                    ESP_LOGI(TAG, "got message length %d: %s", (int)len - 1, &(msg[1]));
                    comms_val.test = atof(&msg[1]);
                    break;
                case 'S':
                    ESP_LOGI(TAG, "got message length %d: %s", (int)len - 1, &(msg[1]));
                    comms_val.speed = atof(&msg[1]);
                    break;
                case 'R':
                    ESP_LOGI(TAG, "got message length %d: %s", (int)len - 1, &(msg[1]));
                    if(strcmp(&msg[1], "true") == 0)
                        comms_val.right_turn = true;
                    else
                        comms_val.right_turn = false;
                    break;
                case 'L':
                    ESP_LOGI(TAG, "got message length %d: %s", (int)len - 1, &(msg[1]));
                    if(strcmp(&msg[1], "true") == 0)
                        comms_val.left_turn = true;
                    else
                        comms_val.left_turn = false;
                    break;
                case 'F':
                    ESP_LOGI(TAG, "got message length %d: %s", (int)len - 1, &(msg[1]));
                    if(strcmp(&msg[1], "true") == 0)
                        comms_val.front = true;
                    else
                        comms_val.front = false;
                    break;
                case 'B':
                    ESP_LOGI(TAG, "got message length %d: %s", (int)len - 1, &(msg[1]));
                    if(strcmp(&msg[1], "true") == 0)
                        comms_val.back = true;
                    else
                        comms_val.back = false;
                    break;
                default:
                    ESP_LOGI(TAG, "got an unknown message with length %d: %s", (int)len, &(msg[1]));
                    break;
            }
        }
        break;
    case WEBSOCKET_BIN:
        ESP_LOGI(TAG, "client %d sent binary message of size %lu :\n%s", num, (uint32_t)len, msg);
        break;
    case WEBSOCKET_PING:
        ESP_LOGI(TAG, "client %d pinged us with message of size %lu :\n%s", num, (uint32_t)len, msg);
        break;
    case WEBSOCKET_PONG:
        ESP_LOGI(TAG, "client %d responded to the ping", num);
        break;
    }
}

static void http_server(struct netconn *conn)
{
    const static char HTML_HEADER[] = "HTTP/1.1 200 OK\nContent-type: text/html\n\n";

    struct netbuf *inbuf;
    static char *buf;
    static uint16_t buflen;
    static err_t err;

    // default page
    extern const uint8_t root_html_start[] asm("_binary_index_html_start");
    extern const uint8_t root_html_end[] asm("_binary_index_html_end");
    const uint32_t root_html_len = root_html_end - root_html_start;

    netconn_set_recvtimeout(conn, 1000); // allow a connection timeout of 1 second
    ESP_LOGI(TAG, "reading from client...");
    err = netconn_recv(conn, &inbuf);
    ESP_LOGI(TAG, "read from client");
    if (err == ERR_OK)
    {
        netbuf_data(inbuf, (void **)&buf, &buflen);
        if (buf)
        {
            // default page
            if (strstr(buf, "GET / ") && !strstr(buf, "Upgrade: websocket"))
            {
                ESP_LOGI(TAG, "Sending /");
                netconn_write(conn, HTML_HEADER, sizeof(HTML_HEADER) - 1, NETCONN_NOCOPY);
                netconn_write(conn, root_html_start, root_html_len, NETCONN_NOCOPY);
                netconn_close(conn);
                netconn_delete(conn);
                netbuf_delete(inbuf);
            }

            // default page websocket
            else if (strstr(buf, "GET / ") && strstr(buf, "Upgrade: websocket"))
            {
                ESP_LOGI(TAG, "Requesting websocket on /");
                ws_server_add_client(conn, buf, buflen, "/", websocket_callback);
                netbuf_delete(inbuf);
            }

            else
            {
                ESP_LOGI(TAG, "Unknown request");
                netconn_close(conn);
                netconn_delete(conn);
                netbuf_delete(inbuf);
            }
        }
        else
        {
            ESP_LOGI(TAG, "Unknown request (empty?...)");
            netconn_close(conn);
            netconn_delete(conn);
            netbuf_delete(inbuf);
        }
    }
    else
    { // if err==ERR_OK
        ESP_LOGI(TAG, "error on read, closing connection");
        netconn_close(conn);
        netconn_delete(conn);
        netbuf_delete(inbuf);
    }
}

static void server_task(void *pvParameters)
{
    // const static char *TAG = "server_task";
    struct netconn *conn, *newconn;
    static err_t err;
    client_queue = xQueueCreate(client_queue_size, sizeof(struct netconn *));

    conn = netconn_new(NETCONN_TCP);
    netconn_bind(conn, NULL, 80);
    netconn_listen(conn);
    ESP_LOGI(TAG, "server listening");
    do
    {
        err = netconn_accept(conn, &newconn);
        ESP_LOGI(TAG, "new client");
        if (err == ERR_OK)
        {
            xQueueSendToBack(client_queue, &newconn, portMAX_DELAY);
            // http_serve(newconn);
        }
        vTaskDelay(10);
    } while (err == ERR_OK);
    netconn_close(conn);
    netconn_delete(conn);
    ESP_LOGE(TAG, "task ending, rebooting board");
    esp_restart();
}

// receives clients from queue, handles them
static void server_handle_task(void *pvParameters)
{
    // const static char *TAG = "server_handle_task";
    struct netconn *conn;
    ESP_LOGI(TAG, "task starting");
    for (;;)
    {
        xQueueReceive(client_queue, &conn, portMAX_DELAY);
        if (!conn)
            continue;
        http_server(conn);
        vTaskDelay(10);
    }
    vTaskDelete(NULL);
}

comms_val_t read_comms()
{
    return comms_val;
}

void reset_val_changed_coms(){
    comms_val.val_changed = false;
}

void start_websocket_server()
{
    ESP_ERROR_CHECK(nvs_flash_init());
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    initialise_mdns();
    netbiosns_init();
    netbiosns_set_name(MDNS_HOST_NAME);

    connect_to_wifi();

    // ESP_ERROR_CHECK(init_fs());
    ws_server_start();
    xTaskCreate(&server_task, "server_task", 3000, NULL, 9, NULL);
    xTaskCreate(&server_handle_task, "server_handle_task", 4000, NULL, 6, NULL);
}