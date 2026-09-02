#ifndef SIGMESSAGES_H
#define SIGMESSAGES_H

/**
 * @file include/os/datacontainer/sigmessages.hpp
 * @brief Data container and persistence definitions for the Home Control Station operating system.
 */



typedef enum {
    STATE_SLAVE_KEEP_ALIVE,
    STATE_NODE_INITIAL_DATA,
    STATE_NODE_DETAILED_DATA
}ClientState;

typedef enum {
    STATE_REQUEST_NODE_INITIAL_DATA,
    STATE_REQUEST_NODE_DETAILED_DATA,
    STATE_KEEP_ALIVE,
    STATE_INVALID
}ServerState;



#endif