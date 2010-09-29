/*$Id: ap_skip.cc,v 21.14 2002/03/26 09:20:25 al Exp $ -*- C++ -*-
 * Copyright (C) 2001 Albert Davis
 * Author: Albert Davis <aldavis@ieee.org>
 *
 * This file is part of "Gnucap", the Gnu Circuit Analysis Package
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
 * 02111-1307, USA.
 *------------------------------------------------------------------
 * collection of functions to skip input
 * all except skip1 skip leading whitespace, skip whatever is being skipped,
 * then skip trailing whitespace.
 */
#include "ap.h"
/*--------------------------------------------------------------------------*/
/* skipbl: skip whitespace.  (any non-graphic character is ws)
 * =,(,) are also ws
 * update string pointer
 * pointer points to first non-space
 * does NOT update _ok flag
 */
CS& CS::skipbl()
{
  while (peek() && (!isgraph(peek()))) {
    skip();
  }
  return *this;
}
/*--------------------------------------------------------------------------*/
/* skip1b: skip 1 matching character and surrounding blanks 
 * _ok = did it
 */
CS& CS::skip1b(char t)
{
  skipbl();
  skip1(t);
  skipbl();
  return *this;
}
/*--------------------------------------------------------------------------*/
/* skip1: skip 1 character matching any in t
 * _ok = did it
 */
CS& CS::skip1(char t)
{
  if (match1(t)) {
    skip();
    assert(_ok);
  }else{
    _ok = false;
  }
  return *this;
}
/*--------------------------------------------------------------------------*/
/* skip1b: skip 1 matching character and surrounding blanks 
 * _ok = did it
 */
CS& CS::skip1b(const std::string& t)
{
  skipbl();
  skip1(t);
  skipbl();
  return *this;
}
/*--------------------------------------------------------------------------*/
/* skip1: skip 1 character matching any in t
 * _ok = did it
 */
CS& CS::skip1(const std::string& t)
{
  if (match1(t)) {
    skip();
    assert(_ok);
  }else{
    _ok = false;
  }
  return *this;
}
/*--------------------------------------------------------------------------*/
/* skiparg: skip an argument (maybe just a comma)
 * _ok = skipped something
 */
CS& CS::skiparg()
{
  int here = cursor();
  if (!skipcom()) {
    if (peek()) {
      skip();
    }
    while (is_alpha() || is_pfloat() || is_argsym()) {
      skip();
    }
    skipcom();
  }
  _ok = cursor() > here;
  return *this;
}
/*--------------------------------------------------------------------------*/
/* skipto: skip to a character (one of ...)
 * _ok = skipped something
 */
CS& CS::skipto1(const std::string& t)
{
  int here = cursor();
  while (ns_more() && !match1(t)) {
    skip();
  }
  _ok = ns_more();
  if (!_ok) {
    reset(here);
  }
  return *this;
}
/*--------------------------------------------------------------------------*/
/* skipto: skip to a character (explicit)
 * _ok = skipped something
 */
CS& CS::skipto1(char c)
{
  int here = cursor();
  while (ns_more() && !match1(c)) {
    skip();
  }
  _ok = ns_more();
  if (!_ok) {
    reset(here);
  }
  return *this;
}
/*--------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------*/
