#ifndef ALLUXIO_FUSE_USERDATA_H
#define ALLUXIO_FUSE_USERDATA_H

#include <stdint.h>
#include <stdbool.h>

#define ALLUXIO_FUSE_MAGIC 0xA11UX10F  // Magic number for type validation

typedef struct alluxio_user_data {
    // Magic number for validation (fixed value)
    uint32_t magic;

    // Version of the userdata structure/logic
    uint32_t version;

    // Function to save FUSE state to a file (e.g., JSON or custom format)
    int (*save_alluxio_fuse_state)();

    // Function to load FUSE state from a file
    int (*restore_alluxio_fuse_state)();

    // Callback to be invoked when the application detects a signal to start migration.
    // libfuse should prepare to pause, save state, and send the fd.
    void (*on_migration_callback)(void *context);

    // Context passed into on_migration_signal (typically a struct fuse *)
    void *migration_callback_context;

    // Function to get the UNIX domain socket path for IPC
    const char *(*get_socket_path)();

    // Function to get the file path where the FUSE state is persisted
    const char *(*get_state_file_path)();

    // Function to determine if this FUSE is launched as a bootstrap receiver
    bool (*should_wait_for_fd)();


    // Function to receive a FUSE /dev/fuse file descriptor via Unix domain socket
    // This function blocks until a valid FD is received or an error occurs.
    int (*recv_fuse_fd)(int *out_fuse_fd);

    // Function to send the current FUSE /dev/fuse file descriptor via Unix domain socket
    // This function blocks until the receiver has accepted the descriptor.
    int (*send_fuse_fd)(int fuse_fd);
} alluxio_user_data;

#endif // ALLUXIO_FUSE_USERDATA_H