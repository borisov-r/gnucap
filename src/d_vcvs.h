/*$Id: d_vcvs.h,v 22.12 2002/07/26 08:02:01 al Exp $ -*- C++ -*-
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
 * voltage controled voltage source
 */
#ifndef D_VCVS_H
#define D_VCVS_H
#include "l_denoise.h"
#include "e_elemnt.h"
/*--------------------------------------------------------------------------*/
class DEV_VCVS : public ELEMENT {
private:
  explicit DEV_VCVS(const DEV_VCVS& p) :ELEMENT(p) {}
public:
  explicit DEV_VCVS()		:ELEMENT() {}
private: // override virtual
  char	   id_letter()const	{return 'E';}
  const char* dev_type()const	{return "vcvs";}
  int	   max_nodes()const	{return 4;}
  int	   min_nodes()const	{return 4;}
  int	   out_nodes()const	{return 2;}
  int	   matrix_nodes()const	{return 4;}
  int	   net_nodes()const	{return 4;}
  bool	   is_2port()const	{return true;}
  CARD*	   clone()const		{return new DEV_VCVS(*this);}
  //void   parse(CS&);		//ELEMENT
  //void   print(OMSTREAM,int)const; //ELEMENT
  //void   expand();		//CARD/nothing
  //void   map_nodes();		//ELEMENT
  void	   precalc();

  void	   tr_alloc_matrix()	{tr_alloc_matrix_extended();}
  void	   dc_begin();
  void	   tr_begin()		{dc_begin();}
  void	   tr_restore()		{dc_begin();}
  //void   dc_advance();	//CARD/nothing
  //void   tr_advance();	//CARD/nothing
  //bool   tr_needs_eval();	//ELEMENT
  //void   tr_queue_eval();	//ELEMENT
  bool	   do_tr();
  void	   tr_load()		{tr_load_loss(); tr_load_active();}
  //double tr_review();		//CARD/nothing
  //void   tr_accept();		//CARD/nothing
  void	   tr_unload()		{tr_unload_active();}
  //double tr_amps()const	//ELEMENT
  double   tr_involts()const	{return dn_diff(_n[IN1].v0(), _n[IN2].v0());}
  double   tr_involts_limited()const {return volts_limited(_n[IN1],_n[IN2]);}
  //double tr_probe_num(CS&)const;//ELEMENT

  void	   ac_alloc_matrix()	{ac_alloc_matrix_extended();}
  void	   ac_begin();
  void	   do_ac();
  void	   ac_load()		{ac_load_loss(); ac_load_active();}
  COMPLEX  ac_involts()const	{return _n[IN1].vac() - _n[IN2].vac();}
  //XPROBE ac_probe_ext(CS&)const;//ELEMENT
};
/*--------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------*/
#endif
