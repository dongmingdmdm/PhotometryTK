// __BEGIN_LICENSE__
// Copyright (C) 2006-2011 United States Government as represented by
// the Administrator of the National Aeronautics and Space Administration.
// All Rights Reserved.
// __END_LICENSE__


#ifndef __PHOTK_TIME_ACCUMULATORS_H__
#define __PHOTK_TIME_ACCUMULATORS_H__

namespace photk {

  // Re-estimate time exposure with reflectance
  class TimeDeltaAccumulator : public vw::ReturnFixedType<void> {
    double m_numerator, m_denominator;
    double m_curr_time;
  public:
    typedef double value_type;

    TimeDeltaAccumulator(double t) : m_numerator(0),
      m_denominator(0), m_curr_time(t) {}

    template <class AValT, class RValT>
    void operator()( AValT const& drg, AValT const& albedo, RValT const& reflectance ) {
      double i = drg[0], a = albedo[0], s = alpha_channel(drg),
        r = vw::compound_select_channel<double>(reflectance,0);
      double inter = a*r;
      m_numerator += (i - m_curr_time*a*r)*inter*s;
      m_denominator += inter*inter*s;
    }

    value_type value() const {
      return m_numerator/m_denominator;
    }
  };

  // Re-estimate time exposure with-out reflectance
  class TimeDeltaNRAccumulator : public vw::ReturnFixedType<void> {
    double m_numerator, m_denominator;
    double m_curr_time;
  public:
    typedef double value_type;

    TimeDeltaNRAccumulator(double t) : m_numerator(0),
      m_denominator(0), m_curr_time(t) {}

    template <class AValT>
    void operator()( AValT const& drg, AValT const& albedo ) {
      double i = drg[0], s = alpha_channel(drg), a = albedo[0];
      m_numerator += (i - m_curr_time*a)*a*s;
      m_denominator += a*a*s;
    }

    value_type value() const {
      if ( m_denominator <= 0 )
	vw::vw_out(vw::WarningMessage) << "No samples available.\n";
      return m_numerator/m_denominator;
    }
  };

}

#endif//__PHOTK_TIME_ACCUMULATORS_H__
