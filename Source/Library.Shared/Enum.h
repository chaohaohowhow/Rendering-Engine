#pragma once

#include <iterator>
#include <exception>

namespace Library
{
	template <typename T>
	class Enum final
	{
	public:
		class Iterator final
		{
		public:
			using size_type = std::size_t;
			using difference_type = std::ptrdiff_t;
			using value_type = T;
			using reference = T;
			using pointer = T*;
			using iterator_category = std::forward_iterator_tag;

			Iterator() = default;
			explicit Iterator(T value);
			Iterator(const Iterator&) = default;
			Iterator(Iterator&&) = default;
			Iterator& operator=(const Iterator&) = default;
			Iterator& operator=(Iterator&&) = default;
			~Iterator() = default;

			T operator*() const;
			Iterator& operator++();
			Iterator operator++(int);

			bool operator==(const Iterator& rhs) const;
			bool operator!=(const Iterator& rhs) const;

		private:
			T mValue;
		};

		Iterator begin() const;
		Iterator end() const;
	};
}

#include "Enum.inl"