/*****************************************************************************
*
* ALPS Project: Algorithms and Libraries for Physics Simulations
*
* ALPS Libraries
*
* Copyright (C) 1994-2004 by Matthias Troyer <troyer@comp-phys.org>,
*                            Fabian Stoeckli <fabstoec@phys.ethz.ch>
*
* This software is part of the ALPS libraries, published under the ALPS
* Library License; you can use, redistribute it and/or modify it under
* the terms of the license, either version 1 or (at your option) any later
* version.
* 
* You should have received a copy of the ALPS Library License along with
* the ALPS Libraries; see the file LICENSE.txt. If not, the license is also
* available from http://alps.comp-phys.org/.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
* FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT 
* SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE 
* FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE, 
* ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER 
* DEALINGS IN THE SOFTWARE.
*
*****************************************************************************/

/* $Id$ */

#ifndef ALPS_ALEA_HISTOGRAMDATA_H
#define ALPS_ALEA_HISTOGRAMDATA_H

#include <alps/alea/histogram.h>
#include <alps/parser/parser.h>
#include <alps/osiris.h>
#include <iostream>
#include <vector>

#ifndef ALPS_WITHOUT_OSIRIS
# include <alps/osiris.h>
#endif

namespace alps {

//==========================================================
//HisogramObservableData
//
//Observable class for Histograms
//----------------------------------------------------------

template <class T>
class HistogramObservableData {
public:
  template <class X> friend class HistogramObservableData;
  typedef uint32_t integer_type;
  typedef integer_type value_type;
  typedef integer_type size_type;
  typedef integer_type count_type;
  typedef T range_type;

    //constructors
  HistogramObservableData();
  HistogramObservableData(const HistogramObservable<T>& obs);
  HistogramObservableData(std::istream& infile, const XMLTag& intag);

  size_type size() const {return histogram_.size();}

  void read_xml(std::istream& infile, const XMLTag& intag);
  void read_xml_histogram(std::istream& infile, const XMLTag& intag);

  ALPS_DUMMY_VOID set_thermalization(uint32_t) {boost::throw_exception(std::runtime_error("no set_thermalization() for HistogramObservableData"));}
  size_type get_thermalization() const { return thermalcount_;} 
  bool can_set_thermalization() const { return false;}

  size_type count() const {return count_;}
  size_type value(uint32_t) const;
  range_type min() const { return min_;}
  range_type max() const { return max_;}
  range_type stepsize() const { return stepsize_;}

  ALPS_DUMMY_VOID compact() {
    count_=count();
    ALPS_RETURN_VOID;
  }	

#ifndef ALPS_WITHOUT_OSIRIS
    void save(ODump& dump) const;
    void load(IDump& dump);
#endif

  //collect many data objects
  void collect_from(const std::vector<HistogramObservableData<T> >& runs);
  value_type operator[](size_type i) const {return histogram_[i];}   

  //std::string evaluation_method(Target t) const;
    
private:
  mutable uint32_t count_;
  mutable bool can_set_thermal_;
  mutable uint32_t thermalcount_;
  //mutable uint32_t discardemeas_;
  //mutable uint32_t discardebins_;
  //bool changed;
  //mutable bool valid_;

  mutable std::vector<value_type> histogram_;
  mutable range_type min_;
  mutable range_type max_;
  mutable range_type stepsize_;
  mutable size_type size_;

};

template <class T>
inline HistogramObservableData<T>::HistogramObservableData()
  :  count_(0),
     can_set_thermal_(false),
     thermalcount_(0),
     histogram_(),
     min_(),
     max_(),
     stepsize_()
{}

template <class T>
inline void HistogramObservableData<T>::collect_from(const std::vector<HistogramObservableData<T> >& runs)
{
  bool got_data=false;
  count_=0;

  for(typename std::vector<HistogramObservableData<T> >::const_iterator r = runs.begin(); r!=runs.end();++r) { 
	if(r->count()) {
	  if(!got_data) {
	    can_set_thermal_=r->can_set_thermal_;
	    thermalcount_=r->thermalcount_;
	    count_=r->count_;
	    thermalcount_=r->thermalcount_;
	    min_=r->min_;
	    max_=r->max_;
	    stepsize_=r->stepsize_;
	    histogram_.resize(r->histogram_.size());
        std::copy(r->histogram_.begin(),r->histogram_.end(),histogram_.begin());
        got_data=true;
      }
	  else {
	    size_type loc_size=histogram_.size();
        if(min_!=r->min_) 
          boost::throw_exception(std::runtime_error("Cannot collect data from histograms with different min_."));
	    if(max_!=r->max_) 
          boost::throw_exception(std::runtime_error("Cannot collect data from histograms with different max_."));
	    if(stepsize_!=r->stepsize_) 
          boost::throw_exception(std::runtime_error("Cannot collect data from histograms with different stepsize_."));
	    if(loc_size!=r->histogram_.size()) 
          boost::throw_exception(std::runtime_error("Cannot collect data from histograms with different size_."));
	    count_+=r->count_;
        std::transform(histogram_.begin(),histogram_.end(),r->histogram_.begin(),histogram_.begin(),std::plus<value_type>());
	  }
	}
  }
}
 

template <class T>
  inline HistogramObservableData<T>::HistogramObservableData(std::istream& infile, const XMLTag& intag)
:count_(0),
     can_set_thermal_(false),
     thermalcount_(0),
     histogram_(),
     min_(),
     max_(),
     stepsize_()
{
  read_xml(infile,intag);
}

template <class T>
  inline void HistogramObservableData<T>::read_xml_histogram(std::istream& infile, const XMLTag& intag)
{
  if (intag.name != "HISTOGRAM") 
    boost::throw_exception(std::runtime_error ("Encountered tag <"+intag.name+"> instead of <HISTOGRAM>"));
  if (intag.type ==XMLTag::SINGLE) 
    return;
  XMLTag tag(intag);
  std::size_t s = boost::lexical_cast<std::size_t,std::string>(tag.attributes["nvalues"]);
  histogram_.resize(s);
  tag = parse_tag(infile);
  int i=0;
  while (tag.name =="ENTRY") {
    tag=parse_tag(infile);
    while (tag.name !="/ENTRY") {
	  if (tag.name=="COUNT") {
        if (tag.type !=XMLTag::SINGLE) {
		  count_=boost::lexical_cast<integer_type, std::string>(parse_content(infile));
		  check_tag(infile,"/COUNT");
        }
      }
	  else if (tag.name=="VALUE") {
        if (tag.type !=XMLTag::SINGLE) {
		  histogram_[i]=static_cast<value_type>(text_to_double(parse_content(infile)));
		  check_tag(infile,"/VALUE");
        }
      }
	  else skip_element(infile,tag);
	  tag = parse_tag(infile);
    }
    ++i;
    tag = parse_tag(infile);
  }
  if (tag.name !="/HISTOGRAM") boost::throw_exception(std::runtime_error("Encountered unknown tag <"+tag.name+"> in <HISTOGRAM>"));
}

template <class T>
  inline HistogramObservableData<T>::HistogramObservableData(const HistogramObservable<T>& obs)
    : count_(obs.count()),
     can_set_thermal_(obs.can_set_thermalization()),
     thermalcount_(obs.get_thermalization()),
     histogram_(obs.size()),
     min_(obs.min()),
     max_(obs.max()),
     stepsize_(obs.stepsize())
{
  if(count())
    for (int i=0;i<obs.size();++i) 
      histogram_[i]=obs[i];
}


template <class T>
  inline void HistogramObservableData<T>::read_xml(std::istream& infile, const XMLTag& intag)
{
  read_xml_histogram(infile,intag);
}


#ifndef ALPS_WITHOUT_OSIRIS

template<class T>
  inline void HistogramObservableData<T>::save(ODump& dump) const
{
  dump <<count_<<histogram_<<min_<<max_<<stepsize_<<thermalcount_<<can_set_thermal_;
}

template <class T>
inline void HistogramObservableData<T>::load(IDump& dump) 
{
dump >>count_>>histogram_>>min_>>max_>>stepsize_>>thermalcount_>>can_set_thermal_;
}

}

#ifndef BOOST_NO_OPERATORS_IN_NAMESPACE
namespace alps {
#endif

template<class T>
inline alps::ODump& operator<<(alps::ODump& od, const alps::HistogramObservableData<T>& m)
{ m.save(od); return od; }

template<class T>
inline alps::IDump& operator>>(alps::IDump& id, alps::HistogramObservableData<T>& m)
{ m.load(id); return id; }

#endif

#ifndef BOOST_NO_OPERATORS_IN_NAMESPACE
} // namespace alps
#endif

#endif
