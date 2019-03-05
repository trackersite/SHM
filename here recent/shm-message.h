/**
* \file shm-message.h
*/

#ifndef SHM_MESSAGE_H
#define SHM_MESSAGE_H

/**
* The size of the message name
*/
#define SHM_MESSAGE_NAME_SIZE 16

/**
* The size of the message name
*/
#define SHM_MESSAGE_TEXT_SIZE 64

/**
* A message.
*/
typedef struct {
  char name[SHM_MESSAGE_NAME_SIZE]; /**< The message name. */
  char text[SHM_MESSAGE_TEXT_SIZE]; /**< The message text. */
} shm_message_t;

/**
 * Copies a source message to a target message.
 * \param message_source The source message.
 * \param message_target The target message.
 * \return -1 on error (i.e., if the target message is not empty, the shm_message_set_name function)
   returns -1 or the message shm_message_set_name function returns -1), 0 else
 */
int shm_message_copy(shm_message_t message_source, shm_message_t *message_target);

 /**
  * Empties a message (i.e., sets the message name and the message text to an empty string).
  * \param message The message.
  */
void shm_message_empty(shm_message_t *message);

 /**
  * Checks if a message is empty (i.e., checks if the message name and the message text are an empty
    string).
  * \param message The message.
  * \return 1 if the message is empty, else 0.
  */
int shm_message_is_empty(shm_message_t message);

/**
 * Prints a message using the "YYYY-MM-DD HH:MM:SS: name: text" format.
 * \param message The message
 */
void shm_message_print(shm_message_t message);

/**
 * Sets the name of a message.
 * \param message The message.
 * \param name The name.
 * \return -1 on error(if the message name size is greater than SHM_MESSAGE_NAME_SIZE), else 0.
 */
int shm_message_set_name(shm_message_t *message, const char *name);

/**
 * Sets the text of a message.
 * \param message The message.
 * \param text The text.
 * \return -1 on error(if the text size is greater than SHM_MESSAGE_TEXT_SIZE), else 0.
 */
int shm_message_set_text(shm_message_t *message, const char *text);
#endif
