#include <stdlib.h>
#include <core/ConsoleController.h>

/*
 * refresh. Returns 1 if successful, 0 otherwise.
 * @desc: Refreshes the screen with a set of controllers, printed one after another.
 * @param: struct ConsoleController *controllers
 * @return: int
 */
// int refresh(Line **lines)
// {
//     Result *controller = (Result *)malloc(sizeof(Result));
//     // Clear the screen
//     clear_screen(controller);
//     if (controller->result == -1)
//     {
//         printf("[ERROR] THERE WAS AN EXCEPTION IN THE REFRESH COMPONENT: %s\n", controller->message);
//         free(controller);
//         return -1;
//     }
//     int componentCounter = sizeof(lines) / sizeof(Line *);
//     printf("There are %d components to refresh\n", componentCounter);
//     for (int i = 0; i < componentCounter; i++)
//     {
//         Line *currentLine = lines[i];
//         while (currentLine != NULL)
//         {
//             set_screen(currentLine->content, controller);
//             currentLine = currentLine->next;
//         }
//         set_screen(lines[i]->content, controller);
//     }
//     free(controller);
//     return 0;
// }
