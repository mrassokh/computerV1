/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Computor.hpp                                        :+:      :+:    :+:  */
/*                                                    +:+ +:+         +:+     */
/*   By: mrassokh <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/27 12:27:19 by mrassokh          #+#    #+#             */
/*   Updated: 2018/06/27 12:27:20 by mrassokh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMPUTOR_HPP
# define COMPUTOR_HPP
# include "CustomExeptions.hpp"
# include <vector>
# include <array>
# include <iostream>
# include <algorithm>
# include <sstream>
# define ABS(x) x < 0 ? -x : x
# define EPS 0.0000001

typedef struct 	s_complex
{
double		real;
double		image;
}		t_complex;

class Computor
{
public:
	static	Computor					&Instance();
	void 							procesing(int argc, char** argv);

private:
	Computor();
	Computor(Computor const & rhs) = delete;
	Computor & operator = (Computor const & rhs) = delete;
	~Computor();

	void 							fillLeftSide(std::shared_ptr<std::vector<double>> leftCoeff,
											std::string const & leftSide);
	void 							fillRightSide(std::shared_ptr<std::vector<double>> rightCoeff,
											std::string const & rightSide);
	void 							fillReducedForm(std::shared_ptr<std::vector<double>> leftCoeff,
											std::shared_ptr<std::vector<double>> rightCoeff,
											std::shared_ptr<std::vector<double>> reducedFormCoeff);
	void 							printReducedForm(std::shared_ptr<std::vector<double>> reducedFormCoeff);

	void 							pushToken(std::shared_ptr<std::vector<double>> coeff,
											std::string & token, char sign);
	void 							pushTokenToright(std::shared_ptr<std::vector<double>> coeff,
											std::string & token, char sign);

	int 							findDegree(std::string const & token);
	void 							trim(std::string& s);
	double							findSquareRoot(double const & square);

	void 							solveZeroDegree(std::shared_ptr<std::vector<double>> reducedFormCoeff);
	void 							solveFirstDegree(std::shared_ptr<std::vector<double>> reducedFormCoeff);
	void 							solveSecondDegree(std::shared_ptr<std::vector<double>> reducedFormCoeff);



	std::array<void (Computor::*)(
		std::shared_ptr<std::vector<double>>), 3>	m_solveFunctions;
	std::shared_ptr<std::vector<double>> 			m_leftSide;
	std::shared_ptr<std::vector<double>> 			m_rightSide;
	std::shared_ptr<std::vector<double>> 			m_reducedForm;
	t_complex						m_firstRoot;
	t_complex						m_secondRoot;
	double							m_discriminant;
	int							m_counter;
	std::string						m_reducedFormStr;
};

#endif
