#ifndef BOOST_DI_EXTENSIONS_H
#define BOOST_DI_EXTENSIONS_H

#include <boost/di.hpp>

template <class TDependency, class TInjector,
    std::enable_if_t<std::is_same<typename TDependency::scope, boost::di::scopes::singleton>::value, int> = 0>
void createSingletonsEagerlyImpl(const boost::di::aux::type<TDependency>&, const TInjector& injector) {
    injector.template create<std::shared_ptr<typename TDependency::expected>>();
}

template <class TDependency, class TInjector,
    std::enable_if_t<!std::is_same<typename TDependency::scope, boost::di::scopes::singleton>::value, int> = 0>
void createSingletonsEagerlyImpl(const boost::di::aux::type<TDependency>&, const TInjector&) { }

template <class... TDeps, class TInjector>
void createSingletonsEagerly(const boost::di::aux::type_list<TDeps...>&, const TInjector& injector) {
    [](...) {}((createSingletonsEagerlyImpl(boost::di::aux::type<TDeps>{}, injector), 0)...);
}

template <class TInjector>
void eagerSingletons(const TInjector& injector) {
    createSingletonsEagerly(typename TInjector::deps{}, injector);
}

#endif // BOOST_DI_EXTENSIONS_H
