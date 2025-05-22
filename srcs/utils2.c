/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anebbou <anebbou@student42.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 13:54:28 by anebbou           #+#    #+#             */
/*   Updated: 2025/05/22 20:55:08 by anebbou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	is_number(const char *str)
{
	if (!str || *str == '\0')
		return (0);
	if (*str == '-' || *str == '+')
		str++;
	while (*str)
	{
		if (*str < '0' || *str > '9')
			return (0);
		str++;
	}
	return (1);
}

long	ft_atol_safe(const char *str)
{
	long	res;
	int		sign;

	res = 0;
	sign = 1;
	if (!is_number(str))
		return (-1);
	if (*str == '-')
		sign = -1;
	if (*str == '-' || *str == '+')
		str++;
	while (*str)
	{
		if (res > (9223372036854775807 - (*str - '0')) / 10)
			return (-1);
		res = res * 10 + (*str - '0');
		str++;
	}
	return (res * sign);
}

int	ft_strcmp(char *s1, char *s2)
{
	while (*s1 && *s2 && *s1 == *s2)
	{
		s1++;
		s2++;
	}
	return ((unsigned char)*s1 - (unsigned char)*s2);
}
