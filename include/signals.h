/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   signal.h                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: lsinke <lsinke@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/08/10 18:31:22 by lsinke        #+#    #+#                 */
/*   Updated: 2022/08/10 18:31:22 by lsinke        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIGNALS_H
# define SIGNALS_H

# include <stdint.h>

/**
 * Store the signal handlers inherited from the parent, and install standard
 * signal handlers. (SIG_IGN for SIGQUIT, standard_interrupt for SIGINT)
 *
 * @return 0 on success, 1 on failure
 */
uint8_t	init_signals(void);

/**
 * Reset the signal handlers back to the handlers inherited from the parent
 *
 * @return 0 on success, 1 on failure
 */
uint8_t	reset_signals(void);

/**
 * Set the standard signal handler for SIGINT
 *
 * Sets g_globals.interrupted to true, and g_globals.exit to 128 + SIGINT when
 * a SIGINT is received
 *
 * @return 0 on success, 1 on failure
 */
uint8_t	signal_standard_interrupt(void);

/**
 * Set the readline signal handler for SIGINT
 *
 * Clears the current line, and starts a new one.
 * Sets g_globals.exit to 128 + SIGINT
 *
 * @return 0 on success, 1 on failure
 */
uint8_t	signal_readline_interrupt(void);

/**
 * Set the signal handler for SIGINT to SIG_IGN
 *
 * @return 0 on success, 1 on failure
 */
uint8_t	signal_ignore_interrupt(void);

#endif