#pragma once
#define MAX_YEARS 41
class CCreditUser
{
public:
	CCreditUser(void);
	CCreditUser(char *);
	~CCreditUser(void);
	double rate;
	double anuity_limit;
	char name[256];

private:
	double cAmmount[MAX_YEARS];
	double cAnuity[MAX_YEARS];
	double cRate[MAX_YEARS];
	double cPlan[MAX_YEARS][MAX_YEARS*12];

public:
	void calculateTotalAmmountonOnRate(void);
	void setTotalAmmount(double ammount);
	void bindUserOnAmmount(CCreditUser *user);
	double getMaxAmmountPerYear(int year);
	void calculateTotalAnuityOnRate(void);
	double getAmmountOnYear(int year);
	double getAnuityOnYear(int year);
	void setAnuityOnYear(int year, double anuity);
        double getRateOnYear(int year);
	void calculateRateOnAmmount(void);
	void AddName(char *usrName);
	void calculateCreditPlan(int year, double *plan);
	double *getCreditPlanOnYear(int year);
};

