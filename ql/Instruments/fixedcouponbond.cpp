/* -*- mode: c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

/*
 Copyright (C) 2004 Jeff Yu
 Copyright (C) 2004 M-Dimension Consulting Inc.
 Copyright (C) 2005, 2006 StatPro Italia srl

 This file is part of QuantLib, a free-software/open-source library
 for financial quantitative analysts and developers - http://quantlib.org/

 QuantLib is free software: you can redistribute it and/or modify it
 under the terms of the QuantLib license.  You should have received a
 copy of the license along with this program; if not, please email
 <quantlib-dev@lists.sf.net>. The license is also available online at
 <http://quantlib.org/reference/license.html>.

 This program is distributed in the hope that it will be useful, but WITHOUT
 ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 FOR A PARTICULAR PURPOSE.  See the license for more details.
*/

#include <ql/Instruments/fixedcouponbond.hpp>
#include <ql/CashFlows/cashflowvectors.hpp>
#include <ql/CashFlows/simplecashflow.hpp>

namespace QuantLib {

    FixedCouponBond::FixedCouponBond(
                    Real faceAmount,
                    const Date& issueDate,
                    const Date& datedDate,
                    const Date& maturityDate,
                    Integer settlementDays,
                    const std::vector<Rate>& coupons,
                    Frequency couponFrequency,
                    const Calendar& calendar,
                    const DayCounter& dayCounter,
                    BusinessDayConvention accrualConvention,
                    BusinessDayConvention paymentConvention,
                    Real redemption,
                    const Handle<YieldTermStructure>& discountCurve,
                    const Date& stub, bool fromEnd, bool longFinal)
    : Bond(faceAmount, dayCounter, calendar, accrualConvention, paymentConvention,
           settlementDays, discountCurve) {

        issueDate_ = issueDate;
        datedDate_ = datedDate;
        maturityDate_ = maturityDate;
        frequency_ = couponFrequency;

        Date firstDate = (fromEnd ? Date() : stub);
        Date nextToLastDate = (fromEnd ? stub : Date());
        Schedule schedule(datedDate, maturityDate, Period(couponFrequency), calendar,
                          accrualConvention, accrualConvention, fromEnd, false, 
                          firstDate, nextToLastDate);

        cashflows_ = FixedRateCouponVector(schedule, paymentConvention,
            std::vector<Real>(1, faceAmount_), coupons, dayCounter);
        // redemption
        Date redemptionDate =
            calendar.adjust(maturityDate, paymentConvention);
        cashflows_.push_back(boost::shared_ptr<CashFlow>(new
            SimpleCashFlow(faceAmount_*redemption/100.0, redemptionDate)));

    }

    #ifndef QL_DISABLE_DEPRECATED
    FixedCouponBond::FixedCouponBond(
                    const Date& issueDate,
                    const Date& datedDate,
                    const Date& maturityDate,
                    Integer settlementDays,
                    const std::vector<Rate>& coupons,
                    Frequency couponFrequency,
                    const Calendar& calendar,
                    const DayCounter& dayCounter,
                    BusinessDayConvention accrualConvention,
                    BusinessDayConvention paymentConvention,
                    Real redemption,
                    const Handle<YieldTermStructure>& discountCurve,
                    const Date& stub, bool fromEnd, bool longFinal)
    : Bond(100.0, dayCounter, calendar, accrualConvention, paymentConvention,
           settlementDays, discountCurve) {

        issueDate_ = issueDate;
        datedDate_ = datedDate;
        maturityDate_ = maturityDate;
        frequency_ = couponFrequency;

        Schedule schedule(calendar, datedDate, maturityDate,
                          couponFrequency, accrualConvention,
                          stub, fromEnd, longFinal);

        cashflows_ = FixedRateCouponVector(schedule, paymentConvention,
            std::vector<Real>(1, faceAmount_), coupons, dayCounter);

        // redemption
        Date redemptionDate =
            calendar.adjust(maturityDate, paymentConvention);
        cashflows_.push_back(boost::shared_ptr<CashFlow>(new
            SimpleCashFlow(faceAmount_*redemption/100.0, redemptionDate)));

    }
    #endif

}
