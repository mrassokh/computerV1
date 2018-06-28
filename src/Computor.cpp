/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Computor.cpp                                        :+:      :+:    :+:  */
/*                                                    +:+ +:+         +:+     */
/*   By: mrassokh <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/27 12:27:33 by mrassokh          #+#    #+#             */
/*   Updated: 2018/06/27 12:27:37 by mrassokh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Computor.hpp"

Computor::Computor()
{
	m_leftSide = std::shared_ptr<std::vector<double>>(new std::vector<double>);
	m_rightSide = std::shared_ptr<std::vector<double>>(new std::vector<double>);
	m_reducedForm = std::shared_ptr<std::vector<double>>(new std::vector<double>);
	m_solveFunctions[0] = &Computor::solveZeroDegree;
	m_solveFunctions[1] = &Computor::solveFirstDegree;
	m_solveFunctions[2] = &Computor::solveSecondDegree;
	m_counter = 0;
	m_reducedFormStr = "Reduced form: ";
}

Computor::~Computor()
{
	m_leftSide->clear();
	m_rightSide->clear();
	m_reducedForm->clear();
}

Computor		&Computor::Instance()
{
	static Computor instance;
	return instance;
}

void 			Computor::procesing(int argc, char** argv)
{
	if (argc != 2)
		throw CustomExeption("Usage: Please input one string as argument");

	std::string inputString = argv[1];
	std::string	leftSideString = "";
	std::string	rightSideString = "";
	size_t	f = std::string::npos;
	f = inputString.find("=");
	if (f != std::string::npos){
		leftSideString = std::string(inputString.begin(), inputString.begin() + f);
		rightSideString = std::string(inputString.begin() + f + 1, inputString.begin() + inputString.size());
	} else {
		throw CustomExeption("Equal sign is not found!!!");
	}

	fillLeftSide(m_leftSide, leftSideString);
	fillRightSide(m_rightSide, rightSideString);
	fillReducedForm(m_leftSide, m_rightSide, m_reducedForm);
	printReducedForm(m_reducedForm);

	if (m_counter > 3){
		std::cout << "Polynomial degree: " << m_counter - 1 << std::endl;
		throw CustomExeption("The polynomial degree is stricly greater than 2, I can't solve.");
	}
	(this->*m_solveFunctions[m_counter - 1])(m_reducedForm);
}


void 			Computor::fillLeftSide(std::shared_ptr<std::vector<double>> leftCoeff,
											std::string const & leftSide)
{
	leftCoeff->clear();
	size_t	f = std::string::npos;
	std::string	token = "";
	std::string str = leftSide;
	char sign = '+';
	trim(str);

	f = str.find_first_of("+-");
	if (f == 0) {
		sign = str[f];
		str = str.substr(f + 1, str.size() - f + 1 );
		f = str.find_first_of("+-");
	}
	while (f != std::string::npos){
		token = std::string(str.begin(), str.begin() + f);
		pushToken(leftCoeff, token, sign);
		sign = str[f];
		str = str.substr(f + 1, str.size() - f + 1 );
		f = str.find_first_of("+-");
	}
	token = std::string(str.begin(), str.begin() +  str.size());
	pushToken(leftCoeff, token,  sign);
	m_counter = leftCoeff->size();
}


void 			Computor::fillRightSide(std::shared_ptr<std::vector<double>> rightCoeff,
												std::string const & rightSide)
{
	rightCoeff->clear();

	int counter = 0;
	while (counter++ < m_counter) {
		rightCoeff->push_back(0);
	}

	size_t	f = std::string::npos;
	std::string	token = "";
	std::string str = rightSide;
	char sign = '+';
	trim(str);

	f = str.find_first_of("+-");
	if (f == 0){
		sign = str[f];
		str = str.substr(f + 1, str.size() - f + 1 );
		f = str.find_first_of("+-");
	}
	while (f != std::string::npos){
		token = std::string(str.begin(), str.begin() + f);
		pushTokenToright(rightCoeff, token, sign);
		sign = str[f];
		str = str.substr(f + 1, str.size() - f + 1 );
		f = str.find_first_of("+-");
	}
	token = std::string(str.begin(), str.begin() +  str.size());
	pushTokenToright(rightCoeff, token, sign);
}

void 			Computor::fillReducedForm(std::shared_ptr<std::vector<double>> leftCoeff,
												std::shared_ptr<std::vector<double>> rightCoeff,
												std::shared_ptr<std::vector<double>> reducedFormCoeff)
{
	if (rightCoeff->size() != leftCoeff->size())
		throw CustomExeption("Wrong degree in right side!!!");

	int counter = 0;
	while (counter++ < m_counter) {
		reducedFormCoeff->push_back(0);
	}
	auto it = reducedFormCoeff->begin();
	counter = 0;
	while (it != reducedFormCoeff->end()){
		(*it) = leftCoeff->at(counter) - rightCoeff->at(counter);
		it++;
		counter++;
	}
}

void 			Computor::printReducedForm(std::shared_ptr<std::vector<double>> reducedFormCoeff)
{
	auto it = reducedFormCoeff->begin();
	int counter = 0;
	while (it != reducedFormCoeff->end()){
		int sign = *it >= 0 ? 1 : 0;
		std::ostringstream strs;
		if (counter)
			strs << (sign ? " + " : " - ")<< (ABS(*it)) << " * X^" << counter;
		else
			strs << (sign ? "" : "-")<< (ABS(*it)) << " * X^" << counter;
		m_reducedFormStr += strs.str();
		it++;
		counter++;
	}
	std::cout << m_reducedFormStr << " = 0" << std::endl;
}

void 			Computor::pushToken(std::shared_ptr<std::vector<double>> coeff,
													std::string & token, char sign)
{
	double num;

	trim(token);
	try {
		num = std::stold(token);
	} catch (std::exception& e){
		if (token[0] == 'X' || token[0] == 'x')
			num = 1;
		else
			throw CustomExeption(e.what());
	}
	if (sign == '+')
		coeff->push_back(num);
	else
		coeff->push_back(-num);
}

void 			Computor::pushTokenToright(std::shared_ptr<std::vector<double>> coeff,
													std::string & token, char sign)
{
	double num;

	trim(token);
	int degree = findDegree(token);
	if (degree > m_counter - 1)
		throw CustomExeption("Wrong degree in right side!!!");
	try {
		num = std::stold(token);
	} catch (std::exception& e) {
		if (token[0] == 'X' || token[0] == 'x')
			num = 1;
		else
			throw CustomExeption(e.what());
	}
	if (sign == '+')
		coeff->at(degree) = num;
	else
		coeff->at(degree) = -num;
}

void 			Computor::trim(std::string& s)
{
	size_t p = s.find_first_not_of(" \t");
	s.erase(0, p);
	p = s.find_last_not_of(" \t");
	if (std::string::npos != p)
		s.erase(p + 1);
}

int 			Computor::findDegree(std::string const & token)
{
	size_t	f = std::string::npos;
	std::string str = token;
	std::string degree = "";
	int num;

	f = str.find("X");
	if (f == std::string::npos)
		return 0;
	f = str.find("^");
	if (f == std::string::npos){
		return 1;
	} else {
		degree = std::string( str.begin() + f + 1,str.begin() +  str.size());
		try {
			num = std::stold(degree);
		} catch (std::exception& e){
			throw CustomExeption(e.what());
		}
		return num;
	}
}

double			Computor::findSquareRoot(double const & square)
{
	double height = 1;
	double width = square;

	while (ABS(width - height) > EPS){
		height = (height + width) / 2;
		width = square / height;
	}

	return height;
}


void 			Computor::solveZeroDegree(std::shared_ptr<std::vector<double>> reducedFormCoeff)
{
	std::cout << "Polynomial degree: " << 0 << std::endl;
	if (reducedFormCoeff->at(0) == 0)
		throw CustomExeption("All the real numbers are solution!!!");
	else
		throw CustomExeption("Incorrect equation!!!Left side in reduced form is not zero");
}

void 			Computor::solveFirstDegree(std::shared_ptr<std::vector<double>> reducedFormCoeff)
{
	std::cout << "Polynomial degree: " << 1 << std::endl;
	if (reducedFormCoeff->at(1) == 0)
		throw CustomExeption("Incorrect equation!!! Division on zero forbidden");
	else if (reducedFormCoeff->at(0) == 0)
		m_firstRoot.real = 0;
	else
		m_firstRoot.real = -reducedFormCoeff->at(0) / reducedFormCoeff->at(1);
	std::cout << "The solution is:\n" << m_firstRoot.real << std::endl;
}

void 			Computor::solveSecondDegree(std::shared_ptr<std::vector<double>> reducedFormCoeff)
{
	double a = reducedFormCoeff->at(2);
	double b = reducedFormCoeff->at(1);
	double c = reducedFormCoeff->at(0);
	double divider = 1 / (2 * a);

	if (a == 0 && b==0){
		solveZeroDegree(reducedFormCoeff);
		return ;
	} else if (a == 0) {
		solveFirstDegree(reducedFormCoeff);
		return ;
	}
	std::cout << "Polynomial degree: " << m_counter - 1 << std::endl;
	std::cout << "a = " << a <<"\n";
	std::cout << "b = " << b <<"\n";
	std::cout << "c = " << c <<"\n";
	m_discriminant = b * b - 4 * a * c;
	std::cout << "discriminant = " << m_discriminant <<"\n";
	if (m_discriminant > 0){
		m_firstRoot.real = (-b + findSquareRoot(m_discriminant)) * divider;
		m_secondRoot.real = (-b - findSquareRoot(m_discriminant)) * divider;
		std::cout << "Discriminant is strictly positive, the two solutions are:\n"
		<< m_firstRoot.real << std::endl << m_secondRoot.real << std::endl;
	} else if (m_discriminant == 0){
		m_firstRoot.real = -b * divider;
		std::cout << "Discriminant is null, the one solutions is:\n"
		<< m_firstRoot.real << std::endl;
	} else {
		m_discriminant = - b * b + 4 * a * c;
		m_firstRoot.real = -b * divider;
		m_firstRoot.image = findSquareRoot(m_discriminant) * divider;
		m_secondRoot.real = -b * divider;
		m_secondRoot.image = -findSquareRoot(m_discriminant) * divider;
		std::cout << "Discriminant is negative, the two complex solutions are:\n"
		<< m_firstRoot.real << (m_firstRoot.image >= 0 ? " + " : " - ") << (ABS(m_firstRoot.image))
		<< " * i" <<	 std::endl
		<< m_secondRoot.real <<(m_secondRoot.image >= 0 ? " + " : " - ") << (ABS(m_secondRoot.image))
		<< " * i" <<	 std::endl;
	}
}
