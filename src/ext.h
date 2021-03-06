// Copyright (c) 2014, 임경현
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// * Redistributions of source code must retain the above copyright notice, this
//   list of conditions and the following disclaimer.
//
// * Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
// OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

/**
 * @file ext.h
 * @date 2015. 10. 23.
 * @author dlarudgus20
 * @copyright The BSD (2-Clause) License
 */


#ifndef EXT_H_
#define EXT_H_

#include <cstddef>
#include <algorithm>
#include <memory>
#include <string>
#include <type_traits>
#include <utility>

namespace ext
{
	// make_unique
	namespace detail
	{
		template <typename T>
		struct unique_if
		{
			typedef std::unique_ptr<T> single_object;
		};
		template <typename T>
		struct unique_if<T[]>
		{
			typedef std::unique_ptr<T[]> unknown_bound;
		};
		template <typename T, std::size_t N>
		struct unique_if<T[N]>
		{
			typedef void known_bound;
		};
	}
	template <typename T, typename... Args>
	typename detail::unique_if<T>::single_object make_unique(Args&&... args)
	{
		return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
	}
	template <typename T>
	typename detail::unique_if<T>::unknown_bound make_unique(size_t n)
	{
		typedef typename std::remove_extent<T>::type U;
		return std::unique_ptr<T>(new U[n]);
	}
	template <typename T, typename... Args>
	typename detail::unique_if<T>::known_bound make_unique(Args&&...) = delete;

	// noncopyable
	class noncopyable
	{
	private:
		noncopyable(const noncopyable &) = delete;
		noncopyable &operator =(const noncopyable &) = delete;
	protected:
		noncopyable() = default;
	};

	// trim
	inline void trimEnd(std::string &str)
	{
		str.erase(
			std::find_if(str.rbegin(), str.rend(),
				[](char c) { return !(isspace(c) || iscntrl(c)); }).base(),
			str.end());
	}
	inline void trimStart(std::string &str)
	{
		str.erase(
			str.begin(),
			std::find_if(str.begin(), str.end(),
				[](char c) { return !(isspace(c) || iscntrl(c)); }));
	}
	inline void trim(std::string &str)
	{
		trimEnd(str);
		trimStart(str);
	}
}

#endif /* EXT_H_ */
