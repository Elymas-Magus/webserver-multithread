#ifndef LOG_CONN_H_INCLUDED
#define LOG_CONN_H_INCLUDED

#include <arpa/inet.h>

#include "utils.h"
#include "client.h"
#include "type_aliases.h"
#include "logs.h"
#include "threadpool_def.h"

/**
 * Register connection start data in log file for debug
 * @param ThreadArg * argsdebug
 * @param Client * client
 * @param String currTime
 */
void logConnectionStart(ThreadArg * args, Client * client, String currTime);

/**
 * Register connection start data in log file for debug
 * @param ThreadArg * argsdebug
 * @param Client * client
 * @param String currTime
 * @param float duration
 * @param String path
 * @param bool error
 */
void logConnectionEnd(ThreadArg * args, Client * client, String currTime, float duration, String path, bool error);

#endif