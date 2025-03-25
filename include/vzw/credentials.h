/** @file credentials.h
 *  @brief Macro and function defines for the vzw thingspace credentials API.
 */

#ifndef CREDENTIALS_H
#define CREDENTIALS_H

/** @fn int vzw_get_auth_token(const char *username, const char *password, const
 *   char *auth_keys, char *auth_token)
 *   @brief Retrieves a Thingspace auth token through a POST request.
 */
int vzw_get_auth_token(const char *auth_keys, char *auth_token);

/** @fn int vzw_get_session_token(const char *username, const char *password, const
 *   char *auth_token, char *session_token)
 *   @brief Retrieves a Thingspace session token through a POST request.
 */
int vzw_get_session_token(
    const char *username, const char *password, const char *auth_token, char *session_token
);
#endif  // CREDENTIALS_H
