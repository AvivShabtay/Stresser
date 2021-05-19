#pragma once

// STRUCT TEMPLATE remove_reference
template <class _Ty>
struct remove_reference
{
	using type = _Ty;
};

template <class _Ty>
struct remove_reference<_Ty&>
{
	using type = _Ty;
};

template <class _Ty>
struct remove_reference<_Ty&&>
{
	using type = _Ty;
};

template <class _Ty>
using remove_reference_t = typename remove_reference<_Ty>::type;


// FUNCTION TEMPLATE move
template <class _Ty>
constexpr remove_reference_t<_Ty>&& move(_Ty&& _Arg) noexcept { // forward _Arg as movable
	return static_cast<remove_reference_t<_Ty>&&>(_Arg);
}