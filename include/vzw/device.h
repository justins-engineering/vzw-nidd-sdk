/** @file device.h
 *  @brief Macro and function defines for the vzw thingspace device API.
 */

#ifndef DEVICE_H
#define DEVICE_H

int vzw_global_device_list(
    const char *account_name, char *auth_token, char *session_token, void *response_data
);

#endif  // DEVICE_H
