#pragma once

// main section patterns
#define begin_main_pt "begin main\n"
#define timesteps_pt "timesteps %d\n"
#define weeks_pt "weeks %d\n"
#define campaigns_pt "campaigns %d\n"
#define scenario_pt "scenario %d\n"
#define epsilon_pt "epsilon %lf\n"
#define powerplant1_pt "powerplant1 %d\n"
#define powerplant2_pt "powerplant2 %d\n"
#define constraint13_pt "constraint13 %d\n"
#define constraint14_pt	"constraint14 %d\n"
#define constraint15_pt	"constraint15 %d\n"
#define constraint16_pt	"constraint16 %d\n"
#define constraint17_pt	"constraint17 %d\n"
#define constraint18_pt	"constraint18 %d\n"
#define constraint19_pt	"constraint19 %d\n"
#define constraint20_pt	"constraint20 %d\n"
#define constraint21_pt "constraint21 %d\n"
#define durations_pt "durations"
#define demand_pt "demand"
#define end_main_pt "end main\n"

// powerplant section patterns
#define begin_powerplant_pt "begin powerplant\n"
#define name_pt "name %s\n"
#define type_pt "type %d\n"
#define index_pt "index %d\n"

#define pmin_pt "pmin"
#define pmax_pt "pmax"
#define cost_pt "cost"
#define stock_pt "stock %d\n"
#define ccMaxModules_pt "current_campaign_max_modulus %d\n"
#define maxModules_pt "max_modulus"
#define maxRefuel_pt "max_refuel"
#define minRefuel_pt "min_refuel"
#define refuelRatio_pt "refuel_ratio"
#define ccStockThreshold_pt "current_campaign_stock_threshold %d\n"
#define stockThreshold_pt "stock_threshold"
#define maxStockBefore_pt "max_stock_before_refueling"
#define maxStockAfter_pt "max_stock_after_refueling"
#define refuelCost_pt "refueling_cost"
#define fuelPrice_pt "fuel_price %lf\n"
#define end_powerplant_pt "end powerplant\n"


#define begin_ccProfile_pt "begin current_campaign_profile\n"
#define beginProfile_pt "begin profile\n"
#define profilePoints_pt "profile_points %d\n"
#define profileCampaignIndex_pt "campaign_profile %d\n"
#define decreaseProfile_pt "decrease_profile"
#define end_ccProfile_pt "end current_campaign_profile\n"
#define endProfile_pt "end profile\n"


#define beginConstraint_pt "begin constraint\n"
#define powerplant_pt "powerplant %d\n"
#define campaign_pt "campaign %d\n"
#define earliestStopTime_pt "earliest_stop_time %d\n"
#define latestStopTime_pt "latest_stop_time %d\n"
#define set_pt "set"
#define spacing_pt "spacing %d\n"
#define start_pt "start %d\n"
#define end_pt "end %d\n"
#define quantity_pt "quantity %d\n"
#define endConstraint_pt "end constraint\n"

#define beginPeriod_pt "begin period\n"
#define ct19Start_pt "start "
#define ct19Duration_pt "duration "
#define endPeriod_pt "end period\n"

#define week_pt "week %d\n"
#define max_pt "max %d\n"
#define startend_pt "startend %d %d\n"
#define maxDouble_pt "max %lf\n"

// ########## solution output patterns ##########
// main section
#define TEAM_IDENTIFIER_PT "team_identifier S%d\n"
#define SOLUTION_DATE_PT "solution_time_date %s\n"
#define SOLUTION_RUNNING_TIME_PT "solution_running_time %s\n"
#define DATA_SET_PT "data_set %s\n"
#define COST_PT "cost %.2f\n"
// outages section
#define BEGIN_OUTAGES_PT "begin outages\n"
#define OUTAGES_DATES_PT "outage_dates"
#define RELOADED_FUEL_PT "reloaded_fuel"
#define END_OUTAGES_PT "end outages\n"
// power output section
#define BEGIN_POWER_OUTPUT_PT "begin power_output\n"
#define BEGIN_TYPE1_PP_PT "begin type1_plants\n"
#define END_TYPE1_PP_PT "end type1_plants\n"
#define BEGIN_TYPE2_PP_PT "begin type2_plants\n"
#define FUEL_VARIATION_PT "fuel_variation"
#define REMAINING_FUEL_PT "remaining_fuel_at_the_end %lf\n"
#define END_TYPE2_PP_PT "end type2_plants\n"
#define END_POWER_OUTPUT_PT "end power_output\n"



