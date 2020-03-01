#ifndef ANALYTIC_MANAGER_FACADE_H
#define ANALYTIC_MANAGER_FACADE_H


class AnalyticManagerFacade
{
public:
    struct SServiceLocator {

    };

    struct SInitSettings {
        SServiceLocator services;
    };

    AnalyticManagerFacade();

    bool init( const SInitSettings & _settings );
    void shutdown();


private:




};

#endif // ANALYTIC_MANAGER_FACADE_H
