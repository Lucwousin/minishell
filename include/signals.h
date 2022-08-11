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

//TODO: this but better
uint8_t	signal_readline(void);
uint8_t	signal_catch_int(void);

#endif