#pragma once

namespace Library
{
	template <typename T>
	inline Enum<T>::Iterator::Iterator(T value) :
		mValue(value)
	{
	}

	template <typename T>
	inline bool Enum<T>::Iterator::operator==(const Iterator& rhs) const
	{
		return mValue == rhs.mValue;
	}

	template <typename T>
	inline bool Enum<T>::Iterator::operator!=(const Iterator& rhs) const
	{
		return mValue != rhs.mValue;
	}

	template <typename T>
	inline T Enum<T>::Iterator::operator*() const
	{
		if (mValue > T::End)
		{
			throw std::runtime_error("Cannot dereference end().");
		}

		return mValue;
	}

	template <typename T>
	inline typename Enum<T>::Iterator& Enum<T>::Iterator::operator++()
	{
		if (mValue <= T::End)
		{
			mValue = T(static_cast<int>(mValue) + 1);
		}
		return *this;
	}

	template <typename T>
	inline typename Enum<T>::Iterator Enum<T>::Iterator::operator++(int)
	{
		Iterator temp = *this;
		operator++();
		return temp;
	}

	template <typename T>
	inline typename Enum<T>::Iterator Enum<T>::begin() const
	{
		return Iterator(T::Begin);
	}

	template <typename T>
	inline typename Enum<T>::Iterator Enum<T>::end() const
	{
		return Iterator(T(static_cast<int>(T::End) + 1));
	}
}

#include "pch.h"