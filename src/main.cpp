/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrassokh <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/27 12:28:11 by mrassokh          #+#    #+#             */
/*   Updated: 2018/06/27 12:28:12 by mrassokh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Computor.hpp"

int main(int argc, char** argv)
{
	try {
		Computor::Instance().procesing(argc,argv);
	} catch (std::exception& e) {
		std::cout << e.what() <<  std::endl;
	}
	return (0);
}
