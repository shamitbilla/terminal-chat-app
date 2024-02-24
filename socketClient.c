#include <ncurses.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <cjson/cJSON.h>

#define PORT 3001
#define MAX_NAME_LENGTH 10
#define MAX_MESSAGE_LENGTH 1024

char name[MAX_NAME_LENGTH];
WINDOW* subWin = NULL;

void main_app();
void calculateStartPosition(int length, int height, int* startX, int* startY);
void *receiveMessages(void *socket_desc);
void render(WINDOW *win, char txt[]);

int main() {
    initscr();

    int height, width, start_main_y, start_main_x;
    height = 13;
    width = 52;
    start_main_y = start_main_x = 10;

    calculateStartPosition(width, height, &start_main_x, &start_main_y);

    WINDOW* main_win = newwin(height, width, start_main_y, start_main_x);
    refresh();
    box(main_win, 0, 0);
    mvwhline(main_win, 4, 1, 0, 50);
    mvwprintw(main_win, 2, 17, "Terminal Chat App");
    mvwprintw(main_win, 6, 4, "Please enter your display name & press enter");
    mvwprintw(main_win, 9, 9, "Your display name: ");
    wrefresh(main_win);
    mvwgetstr(main_win, 9, 28, name);

    endwin();
    clear();

    main_app();

    return 0;
}

void *receiveMessages(void *socket_desc) {
    int clientSocket = *((int *)socket_desc);
    char server_message[2000];

    while (1) {
        int bytes_received = recv(clientSocket, server_message, sizeof(server_message), 0);
        if (bytes_received < 0) {
            printf("Error while receiving server's msg\n");
            break;
        } else if (bytes_received == 0) {
            printf("Server disconnected\n");
            break;
        }

        if (strlen(server_message) > 0) {
            cJSON *parsed_message = cJSON_Parse(server_message);
            if (parsed_message != NULL) {
                cJSON *name_obj = cJSON_GetObjectItem(parsed_message, "name");
                cJSON *msg_obj = cJSON_GetObjectItem(parsed_message, "message");
                if (name_obj != NULL && msg_obj != NULL && cJSON_IsString(name_obj) && cJSON_IsString(msg_obj)) {
                    char formatted_msg[2000];
                    snprintf(formatted_msg, sizeof(formatted_msg), "%s: %s", name_obj->valuestring, msg_obj->valuestring);
                    render(subWin, formatted_msg);
                } else {
                    printf("Error accessing name/message field\n");
                }
                cJSON_Delete(parsed_message);
            }
            memset(server_message, '\0', sizeof(server_message));
        }
    }

    close(clientSocket);
    pthread_exit(NULL);
}

void main_app() {
    int socket_desc;
    struct sockaddr_in server_addr;
    char client_message[2000];
    memset(client_message, '\0', sizeof(client_message));

    socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_desc < 0) {
        printf("Unable to create socket\n");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(socket_desc, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        printf("Unable to connect\n");
        exit(EXIT_FAILURE);
    }

    pthread_t recv_thread;
    if (pthread_create(&recv_thread, NULL, receiveMessages, &socket_desc) != 0) {
        printf("Failed to create receive thread\n");
        exit(EXIT_FAILURE);
    }

    char message[MAX_MESSAGE_LENGTH];
    char prefix[] = "Me: ";
    initscr();

    int height, width, start_chat_y, start_chat_x;
    height = 50;
    width = 110;
    start_chat_y = start_chat_x = 10;

    calculateStartPosition(width, height, &start_chat_x, &start_chat_y);
    WINDOW* chat_win = newwin(height, width, start_chat_y, start_chat_x);
    subWin = subwin(chat_win, 38, 106, 5, 3);
    //box(subWin, 0, 0);
    wrefresh(subWin);
    refresh();
    box(chat_win, 0, 0);
    mvwhline(chat_win, 4, 1, 0, 108);
    mvwprintw(chat_win, 2, 46, "Terminal Chat App");
    mvwhline(chat_win, 43, 1, 0, 108);
    mvwprintw(chat_win, 44, 2, ">");
    wrefresh(chat_win);
    refresh();

    while (1) {
        mvwgetstr(chat_win, 44, 4, message);
        message[strcspn(message, "\n")] = '\0';
        cJSON *json = cJSON_CreateObject();
        if (json == NULL) {
            printf("Error creating JSON object\n");
            break;
        }

        cJSON_AddStringToObject(json, "name", name);
        cJSON_AddStringToObject(json, "message", message);

        char rendertxt[strlen(prefix) + strlen(message) + 1]; // Allocate enough space
        strcpy(rendertxt, prefix); // Copy prefix into rendertxt
        strcat(rendertxt, message); // Concatenate message to rendertxt

        render(subWin, rendertxt); // Render the concatenated string

        char *json_str = cJSON_Print(json);
        if (json_str == NULL) {
            printf("Error converting JSON to string\n");
            cJSON_Delete(json);
            break;
        }

        if (strcmp(message, "exit") == 0) {
            printf("Exiting...\n");
            endwin();
            exit(EXIT_SUCCESS);
        }

        if (send(socket_desc, json_str, strlen(json_str), 0) < 0) {
            printf("Unable to send message\n");
            break;
        }

        memset(message, '\0', sizeof(message));
        wmove(chat_win, 44, 4);
        wclrtoeol(chat_win);
        wrefresh(chat_win);
    }

    pthread_join(recv_thread, NULL);
    close(socket_desc);
    endwin();
}

void calculateStartPosition(int length, int height, int* startX, int* startY) {
    int yMax, xMax;
    getmaxyx(stdscr, yMax, xMax);
    int halfLength = length / 2;
    int halfHeight = height / 2;
    *startX = (xMax / 2) - halfLength;
    *startY = (yMax / 2) - halfHeight;
}

void render(WINDOW *win, char txt[]) 
{
    scrollok(win, TRUE);
    wprintw(win, "%s\n\n", txt);
    wrefresh(win);
}
