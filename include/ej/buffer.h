/**
 * Buffers.
 *
 * This header defines the buffer type.
 *
 * \copyright Justin Handville 2018.  All rights reserved.  Please see LICENSE
 *            for licensing.
 */
#ifndef  EJ_BUFFER_HEADER_GUARD
# define EJ_BUFFER_HEADER_GUARD

#include <ej/allocator.h>
#include <ej/commandfwd.h>
#include <ej/disposable.h>
#include <ej/list.h>
#include <ej/stack.h>
#include <ej/string.h>

#ifdef   __cplusplus
extern "C" {
#endif /*__cplusplus*/

/**
 * A buffer contains a linked list of strings, a command stack, and a command
 * queue.
 */
typedef struct buffer
{
    disposable_t hdr;
    allocator_t* allocator;
    list_t* lines;
    command_stack_t* undo_commands;
    command_queue_t* redo_commands;
} buffer_t;

/**
 * Initialize a buffer from an allocator, a line list, a command stack,
 * and a command queue.
 *
 * Buffer takes ownership of the line list, the command stack, and the command
 * queue. It will dispose() these when done, and it assumes that the allocator
 * can free these items.
 *
 * \param buffer            The buffer to initialize;
 * \param allocator         The allocator to use for allocating lines and
 *                          freeing the structures it takes ownership of.
 * \param lines             The lines to assign to this buffer, or NULL to
 *                          create an empty buffer.
 * \param undo_commands     A command stack to use to undo commands applied to
 *                          the lines in this buffer.
 * \param redo_commands     A command queue to use to redo commands applied to
 *                          the lines in this buffer.
 *
 * \returns 0 if this structure was successfully initialized, or non-zero on
 *          failure.
 */
int buffer_init(
    buffer_t* buffer, allocator_t* allocator, list_t* lines,
    command_stack_t* undo_commands, command_queue_t* redo_commands);

#ifdef   __cplusplus
}
#endif /*__cplusplus*/

#endif /*EJ_BUFFER_HEADER_GUARD*/
