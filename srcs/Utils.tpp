/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andvieir <andvieir@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/05 19:23:59 by andvieir          #+#    #+#             */
/*   Updated: 2024/02/05 19:23:59 by andvieir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/webserv.hpp"

/**
 * @brief Inverts the order of elements in a stack.
 *
 * This function takes a stack and reverses the order of its elements.
 * It achieves this by transferring all elements from the original stack to a temporary stack,
 * and then transferring them back to the original stack in reversed order.
 *
 * @tparam T The type of elements stored in the stack.
 * @param original The original stack to be inverted.
 */
template <typename T>
void	invertStack(std::stack<T>& original) {
	std::stack<T> tempStack;

	while (!original.empty()) {
		tempStack.push(original.top());
		original.pop();
	}
	while (!tempStack.empty()) {
		original.push(tempStack.top());
		tempStack.pop();
	}
}

/**
 * @brief Inverts the elements in a vector.
 *
 * @tparam T The type of elements in the vector.
 * @param original The vector to be inverted.
 */
template <typename T>
void	invertVector(std::vector<T>& original) {
	typename std::vector<T>::size_type left = 0;
	typename std::vector<T>::size_type right = original.size() - 1;
	if (original.empty()) {
		std::cout << "No servers active - Awaiting termination..." << std::endl;
		return ;
	}
	while (left < right) {
		T temp = original[left];
		original[left] = original[right];
		original[right] = temp;
		++left;
		--right;
	}
}
