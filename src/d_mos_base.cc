/* $Id: d_mos_base.model,v 21.14 2002/03/26 09:20:25 al Exp $ -*- C++ -*-
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
 * MOS model - base for levels 1,2,3,6
 */ 
/* This file is automatically generated. DO NOT EDIT */

  const double mDEFAULT_is = 1e-14;
#include "ap.h"
#include "d_mos_base.h"
/*--------------------------------------------------------------------------*/
const double NA(NOT_INPUT);
const double INF(BIGBIG);
/*--------------------------------------------------------------------------*/
int MODEL_MOS_BASE::_count = 0;
/*--------------------------------------------------------------------------*/
SDP_MOS_BASE::SDP_MOS_BASE(const COMMON_COMPONENT* cc)
  :SDP_DIODE(cc)
{
  assert(cc);
  const COMMON_MOS* c = prechecked_cast<const COMMON_MOS*>(cc);
  assert(c);
  const MODEL_MOS_BASE* m = prechecked_cast<const MODEL_MOS_BASE*>(c->model());
  assert(m);
  l_eff = (c->l_in != NA) ? c->l_in : OPT::defl;
  w_eff = (c->w_in != NA) ? c->w_in : OPT::defw;
  ad = (c->ad_in != NA) ? c->ad_in : OPT::defad;
  as = (c->as_in != NA) ? c->as_in : OPT::defas;
  cgate = NA;
  phi = NA;

      {if (m->rsh != NA  &&  m->rd <= 0.  &&  m->rs <= 0.){
	rd = m->rsh * c->nrd;
	rs = m->rsh * c->nrs;
      }else{
	untested();
	rd = (m->rd != NA) ? m->rd : 0.;
	rs = (m->rs != NA) ? m->rs : 0.;
      }}
      {if (m->js == NA  ||  ad == 0.  ||  as == 0.){
	{if (m->is == NA){
	  untested();
	  idsat = issat = mDEFAULT_is;
	  //error(bWARNING, "ignoring js, using default is\n");
	}else{
	  idsat = issat = m->is;	/* this convoluted logic */
	}}				/* is for Spice compatibility */
      }else{
	idsat = m->js * ad;
	issat = m->js * as;
      }}
}
/*--------------------------------------------------------------------------*/
TDP_MOS_BASE::TDP_MOS_BASE(const DEV_MOS*)
{
}
/*--------------------------------------------------------------------------*/
MODEL_MOS_BASE::MODEL_MOS_BASE()
  :MODEL_DIODE(),
   level(1),
   wmax(INF),
   wmin(0),
   lmax(INF),
   lmin(0),
   is(NA),
   js(NA),
   rsh(NA),
   rd(NA),
   rs(NA),
   cbd(NA),
   cbs(NA),
   cgso(0.0),
   cgdo(0.0),
   cgbo(0.0),
   cmodel(0),
   polarity(pN),
   egap(NA),
   needs_isub(false)
{
  ++_count;
  _tnom = OPT::tnom;
}
/*--------------------------------------------------------------------------*/
bool MODEL_MOS_BASE::parse_front(CS& cmd)
{
  return set(cmd, "NMOS", &polarity, pN)
    || set(cmd, "PMOS", &polarity, pP);
}
/*--------------------------------------------------------------------------*/
void MODEL_MOS_BASE::parse_params(CS& cmd)
{
  get(cmd, "TNOM", &_tnom, mOFFSET, -ABS_ZERO);
  get(cmd, "FC", &fc);
  get(cmd, "PB", &pb, mPOSITIVE);
  get(cmd, "CJ", &cjo);
  get(cmd, "MJ", &mj);
  get(cmd, "CJSW", &cjsw);
  get(cmd, "PBSW", &pbsw);
  get(cmd, "MJSW", &mjsw);
  get(cmd, "KF", &kf);
  get(cmd, "AF", &af);
  get(cmd, "LEVEL", &level);
  get(cmd, "WMAX", &wmax, mPOSITIVE);
  get(cmd, "WMIN", &wmin, mPOSITIVE);
  get(cmd, "LMAX", &lmax, mPOSITIVE);
  get(cmd, "LMIN", &lmin, mPOSITIVE);
  get(cmd, "IS", &is);
  get(cmd, "JS", &js);
  get(cmd, "RSH", &rsh);
  get(cmd, "RD", &rd);
  get(cmd, "RS", &rs);
  get(cmd, "CBD", &cbd);
  get(cmd, "CBS", &cbs);
  get(cmd, "CGSo", &cgso);
  get(cmd, "CGDo", &cgdo);
  get(cmd, "CGBo", &cgbo);
  get(cmd, "CMODEL", &cmodel);
}
/*--------------------------------------------------------------------------*/
void MODEL_MOS_BASE::parse_finish()
{
  _tnom = std::max(_tnom, 1.0);
  egap = 1.16 - (7.02e-4*_tnom*_tnom) / (_tnom+1108.);

      {if ((rs == NA)  &&  (rd != NA)){
	untested();
	error(bWARNING,
	      long_label() + ": rd input, but not rs. setting rs = 0.\n");
	rs = 0.;
      }else if ((rd == NA)  &&  (rs != NA)){
	untested();
	error(bWARNING, 
	      long_label() + ": rs input, but not rd. setting rd = 0.\n");
	rd = 0.;
      }else{
	/* rd, rs are ok, either have both or neither */
      }}
      
      {if ((rsh != NA)  &&  (rd != NA)){
	error(bWARNING, long_label() + ": rsh - rs - rd conflict: using "
	      + (((rd <= 0.)  &&  (rs <= 0.)) ? "rsh" : "rs,rd") + '\n');
      }else if ((rsh == NA)  &&  (rd == NA)){
	rsh = 0.;
      }else{
	/* rsh, rd are ok, have one or other */
      }}
      
      {if (is == NA  &&  js == NA){
	is = mDEFAULT_is;
      }else if (is != NA  &&  js != NA){
	error(bWARNING, long_label() + ": is - js conflict\n");
      }else{
	/* is, js are ok, have one or other */
      }}
}
/*--------------------------------------------------------------------------*/
SDP_CARD* MODEL_MOS_BASE::new_sdp(const COMMON_COMPONENT* c)const
{
  assert(c);
  {if (dynamic_cast<const COMMON_MOS*>(c)) {
    return new SDP_MOS_BASE(c);
  }else{
    return MODEL_DIODE::new_sdp(c);
  }}
}
/*--------------------------------------------------------------------------*/
void MODEL_MOS_BASE::print_front(OMSTREAM& o)const
{
  {if (polarity == pN) {
    o << "  nmos";
  }else if (polarity == pP) {
    o << "  pmos";
  }else{
    unreachable();
  }}
}
/*--------------------------------------------------------------------------*/
void MODEL_MOS_BASE::print_params(OMSTREAM& o)const
{
  o << "  tnom=" << _tnom-(-ABS_ZERO);
  o << "  fc=" << fc;
  o << "  pb=" << pb;
  o << "  cj=" << cjo;
  o << "  mj=" << mj;
  o << "  cjsw=" << cjsw;
  if (pbsw != pb)
    o << "  pbsw=" << pbsw;
  o << "  mjsw=" << mjsw;
  if (kf != NA)
    o << "  kf=" << kf;
  if (af != NA)
    o << "  af=" << af;
  if (false)
    o << "  level=" << level;
  if (wmax<INF)
    o << "  wmax=" << wmax;
  if (wmin>0)
    o << "  wmin=" << wmin;
  if (lmax<INF)
    o << "  lmax=" << lmax;
  if (lmin>0)
    o << "  lmin=" << lmin;
  if (is != NA)
    o << "  is=" << is;
  if (js != NA)
    o << "  js=" << js;
  if (rsh != NA)
    o << "  rsh=" << rsh;
  if (rd != NA)
    o << "  rd=" << rd;
  if (rs != NA)
    o << "  rs=" << rs;
  if (cbd != NA)
    o << "  cbd=" << cbd;
  if (cbs != NA)
    o << "  cbs=" << cbs;
  o << "  cgso=" << cgso;
  o << "  cgdo=" << cgdo;
  o << "  cgbo=" << cgbo;
  if (false)
    o << "  cmodel=" << cmodel;
}
/*--------------------------------------------------------------------------*/
void MODEL_MOS_BASE::print_calculated(OMSTREAM& o)const
{
  o << "";
}
/*--------------------------------------------------------------------------*/
bool MODEL_MOS_BASE::is_valid(const COMMON_COMPONENT* cc)const
{
  const COMMON_MOS* c = dynamic_cast<const COMMON_MOS*>(cc);
  {if (!c) {
    return MODEL_DIODE::is_valid(cc);
  }else{
    const MODEL_MOS_BASE* m = this;
    return c->l_in >= m->lmin && c->l_in <= m->lmax
      && c->w_in >= m->wmin && c->w_in <= m->wmax;
  }}
}
/*--------------------------------------------------------------------------*/
void MODEL_MOS_BASE::tr_eval(COMPONENT*)const
{
}
/*--------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------*/
