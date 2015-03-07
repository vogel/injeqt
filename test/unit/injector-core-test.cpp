/*
 * %injeqt copyright begin%
 * Copyright 2014 Rafał Malinowski (rafal.przemyslaw.malinowski@gmail.com)
 * %injeqt copyright end%
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include "exception/ambiguous-types.cpp"
#include "exception/default-constructor-not-found.cpp"
#include "exception/dependency-duplicated.cpp"
#include "exception/dependency-on-self.cpp"
#include "exception/dependency-on-subtype.cpp"
#include "exception/dependency-on-supertype.cpp"
#include "exception/empty-type.cpp"
#include "exception/exception.cpp"
#include "exception/instantiation-failed.cpp"
#include "exception/interface-not-implemented.cpp"
#include "exception/invalid-dependency.cpp"
#include "exception/invalid-qobject.cpp"
#include "exception/invalid-setter.cpp"
#include "exception/qobject-type.cpp"
#include "exception/unavailable-required-types.cpp"
#include "exception/unknown-type.cpp"
#include "exception/unique-factory-method-not-found.cpp"
#include "exception/unresolvable-dependencies.cpp"
#include "dependencies.cpp"
#include "dependency.cpp"
#include "implementation.cpp"
#include "implemented-by.cpp"
#include "injector-core.cpp"
#include "interfaces-utils.cpp"
#include "required-to-satisfy.cpp"
#include "resolved-dependency.cpp"
#include "resolve-dependencies.cpp"
#include "setter-method.cpp"
#include "type.cpp"
#include "type-dependencies.cpp"
#include "type-relations.cpp"
#include "types-by-name.cpp"
#include "types-model.cpp"

#include "../mocks/mocked-provider.h"
#include "expect.h"
#include "utils.h"

#include <QtTest/QtTest>
#include <string>

using namespace injeqt::internal;
using namespace injeqt::v1;

class type_1 : public QObject
{
	Q_OBJECT
};

class type_1_subtype_1 : public type_1
{
	Q_OBJECT
};

class type_1_subtype_2 : public type_1
{
	Q_OBJECT
};

class type_2 : public QObject
{
	Q_OBJECT

public:
	type_1 *o = nullptr;

private slots:
	INJEQT_SETTER void set_type_1(type_1 *x) { o = x; }
};

class type_3 : public QObject
{
	Q_OBJECT
};

class type_4;
class type_5;
class type_6;

class type_4 : public QObject
{
	Q_OBJECT

public:
	type_5 *o = nullptr;

private slots:
	INJEQT_SETTER void set_type_5(type_5 *x) { o = x; }
};

class type_5 : public QObject
{
	Q_OBJECT

public:
	type_6 *o = nullptr;

private slots:
	INJEQT_SETTER void set_type_6(type_6 *x) { o = x; }
};

class type_6 : public QObject
{
	Q_OBJECT

public:
	type_4 *o = nullptr;

private slots:
	INJEQT_SETTER void set_type_4(type_4 *x) { o = x; }
};

class type_7 : public QObject
{
	Q_OBJECT
};

class type_8 : public QObject
{
	Q_OBJECT
};

class type_9 : public QObject
{
	Q_OBJECT

public:
	type_7 *o7 = nullptr;
	type_8 *o8 = nullptr;

private slots:
	INJEQT_SETTER void set_type_7(type_7 *x) { o7 = x; }
	INJEQT_SETTER void set_type_8(type_8 *x) { o8 = x; }
};


class unregistered_type : public QObject
{
	Q_OBJECT

public:
	type_1 *o1;
	type_2 *o2;

	unregistered_type() : QObject{}, o1{nullptr}, o2{nullptr} {}

private slots:
	INJEQT_SETTER void set_type_1(type_1 *x) { o1 = x; }
	INJEQT_SETTER void set_type_1(type_2 *x) { o2 = x; }
};

class injector_core_test : public QObject
{
	Q_OBJECT

private:
	template<typename T>
	T * get(injector_core &i) { return qobject_cast<T *>(i.get(make_type<T>())); }

private slots:
	void should_create_empty_injector_core();
	void should_create_simple_injector_core_and_return_object();
	void should_not_accept_doubled_type();
	void should_not_accept_type_and_subtype();
	void should_not_accept_subtype_and_type();
	void should_accept_two_subtypes();
	void should_not_accept_unresolvable_dependency();
	void should_accept_resolvable_dependency();
	void should_accept_resolvable_supertype_dependency();
	void should_not_accept_ambiguous_supertype_dependency();
	void should_not_accept_unknown_required_type();
	void should_accept_known_required_type();
	void should_accept_known_required_supertype();
	void should_not_accept_ambiguous_required_supertype();
	void should_accept_cyclic_dependencies();
	void should_accept_dependencies_that_are_required();
	void should_inject_into_unregistered_type();
	void should_not_inject_into_when_unknown_dependencies();
	// TODO: https://github.com/vogel/injeqt/issues/3
	/*
		void should_not_accept_cyclic_required_types();
		void should_not_accept_dependent_as_required_type();
	*/

};

void injector_core_test::should_create_empty_injector_core()
{
	auto i = injector_core{};
	expect<exception::unknown_type>({"type_1"}, [&](){
		i.get(make_type<type_1>());
	});
}

void injector_core_test::should_create_simple_injector_core_and_return_object()
{
	auto type_1_provider_p = make_mocked_provider<type_1>();
	auto type_1_provider = type_1_provider_p.get();
	auto configuration = std::vector<std::unique_ptr<provider>>{};
	configuration.push_back(std::move(type_1_provider_p));

	auto i = injector_core{types_by_name{}, std::move(configuration)};
	QVERIFY(type_1_provider->object() == nullptr);

	auto o1 = get<type_1>(i);
	auto o2 = get<type_1>(i);

	QVERIFY(o1 != nullptr);
	QVERIFY(o1 == o2);
	QVERIFY(type_1_provider->object() == o1);
}

void injector_core_test::should_not_accept_doubled_type()
{
	auto configuration = std::vector<std::unique_ptr<provider>>{};
	configuration.push_back(make_mocked_provider<type_1>());
	configuration.push_back(make_mocked_provider<type_1>());

	expect<exception::ambiguous_types>({"type_1"}, [&](){
		auto i = injector_core{types_by_name{}, std::move(configuration)};
	});
}

void injector_core_test::should_not_accept_type_and_subtype()
{
	auto configuration = std::vector<std::unique_ptr<provider>>{};
	configuration.push_back(make_mocked_provider<type_1>());
	configuration.push_back(make_mocked_provider<type_1_subtype_1>());

	expect<exception::ambiguous_types>({"type_1", "type_1_subtype_1"}, [&](){
		auto i = injector_core{types_by_name{}, std::move(configuration)};
	});
}

void injector_core_test::should_not_accept_subtype_and_type()
{
	auto configuration = std::vector<std::unique_ptr<provider>>{};
	configuration.push_back(make_mocked_provider<type_1_subtype_1>());
	configuration.push_back(make_mocked_provider<type_1>());

	expect<exception::ambiguous_types>({"type_1", "type_1_subtype_1"}, [&](){
		auto i = injector_core{types_by_name{}, std::move(configuration)};
	});
}

void injector_core_test::should_accept_two_subtypes()
{
	auto configuration = std::vector<std::unique_ptr<provider>>{};
	configuration.push_back(make_mocked_provider<type_1_subtype_1>());
	configuration.push_back(make_mocked_provider<type_1_subtype_2>());

	auto i = injector_core{types_by_name{}, std::move(configuration)};
	auto o1 = get<type_1_subtype_1>(i);
	auto o2 = get<type_1_subtype_2>(i);

	QVERIFY(o1 != nullptr);
	QVERIFY(o2 != nullptr);

	expect<exception::unknown_type>({"type_1"}, [&](){
		get<type_1>(i);
	});
}

void injector_core_test::should_not_accept_unresolvable_dependency()
{
	auto configuration = std::vector<std::unique_ptr<provider>>{};
	configuration.push_back(make_mocked_provider<type_2>());

	expect<exception::invalid_setter>({"set_type_1"}, [&](){ // TODO: figure out better exception name
		auto i = injector_core{types_by_name{}, std::move(configuration)};
	});
}

void injector_core_test::should_accept_resolvable_dependency()
{
	auto type_1_provider_p = make_mocked_provider<type_1>();
	auto type_1_provider = type_1_provider_p.get();
	auto type_2_provider_p = make_mocked_provider<type_2>();
	auto type_2_provider = type_2_provider_p.get();

	auto configuration = std::vector<std::unique_ptr<provider>>{};
	configuration.push_back(std::move(type_1_provider_p));
	configuration.push_back(std::move(type_2_provider_p));

	auto i = injector_core{types_by_name{make_type<type_1>(), make_type<type_2>()}, std::move(configuration)};
	QVERIFY(type_1_provider->object() == nullptr);
	QVERIFY(type_2_provider->object() == nullptr);

	auto o2 = get<type_2>(i);
	auto o1 = type_1_provider->object();
	QVERIFY(o1 != nullptr);
	QVERIFY(o2 != nullptr);
	QVERIFY(o1 == get<type_1>(i));
	QVERIFY(o1 == o2->o);
}

void injector_core_test::should_accept_resolvable_supertype_dependency()
{
	auto type_1_provider_p = make_mocked_provider<type_1_subtype_1>();
	auto type_1_provider = type_1_provider_p.get();
	auto type_2_provider_p = make_mocked_provider<type_2>();
	auto type_2_provider = type_2_provider_p.get();

	auto configuration = std::vector<std::unique_ptr<provider>>{};
	configuration.push_back(std::move(type_1_provider_p));
	configuration.push_back(std::move(type_2_provider_p));

	auto i = injector_core{types_by_name{make_type<type_1>(), make_type<type_1_subtype_1>(), make_type<type_2>()}, std::move(configuration)};
	QVERIFY(type_1_provider->object() == nullptr);
	QVERIFY(type_2_provider->object() == nullptr);

	auto o2 = get<type_2>(i);
	auto o1 = type_1_provider->object();
	QVERIFY(o1 != nullptr);
	QVERIFY(o2 != nullptr);
	QVERIFY(o1 == get<type_1>(i));
	QVERIFY(o1 == get<type_1_subtype_1>(i));
	QVERIFY(o1 = o2->o);
}

void injector_core_test::should_not_accept_ambiguous_supertype_dependency()
{
	auto configuration = std::vector<std::unique_ptr<provider>>{};
	configuration.push_back(make_mocked_provider<type_1_subtype_1>());
	configuration.push_back(make_mocked_provider<type_1_subtype_2>());
	configuration.push_back(make_mocked_provider<type_2>());

	expect<exception::invalid_setter>({"set_type_1"}, [&](){ // better exception name
		auto i = injector_core{types_by_name{}, std::move(configuration)};
	});
}

void injector_core_test::should_not_accept_unknown_required_type()
{
	auto configuration = std::vector<std::unique_ptr<provider>>{};
	configuration.push_back(make_mocked_provider<type_3, type_1>());

	expect<exception::unavailable_required_types>({"type_1", "type_3"}, [&](){
		auto i = injector_core{types_by_name{}, std::move(configuration)};
	});
}

void injector_core_test::should_accept_known_required_type()
{
	auto type_1_provider_p = make_mocked_provider<type_1>();
	auto type_1_provider = type_1_provider_p.get();
	auto type_3_provider_p = make_mocked_provider<type_3, type_1>();
	auto type_3_provider = type_3_provider_p.get();

	auto configuration = std::vector<std::unique_ptr<provider>>{};
	configuration.push_back(std::move(type_1_provider_p));
	configuration.push_back(std::move(type_3_provider_p));

	auto i = injector_core{types_by_name{}, std::move(configuration)};
	QVERIFY(type_1_provider->object() == nullptr);
	QVERIFY(type_3_provider->object() == nullptr);

	auto o3 = get<type_3>(i);
	auto o1 = type_1_provider->object();
	QVERIFY(o3 != nullptr);
	QVERIFY(o1 != nullptr);
	QVERIFY(o1 == get<type_1>(i));
}

void injector_core_test::should_accept_known_required_supertype()
{
	auto type_1_provider_p = make_mocked_provider<type_1_subtype_1>();
	auto type_1_provider = type_1_provider_p.get();
	auto type_3_provider_p = make_mocked_provider<type_3, type_1>();
	auto type_3_provider = type_3_provider_p.get();

	auto configuration = std::vector<std::unique_ptr<provider>>{};
	configuration.push_back(std::move(type_1_provider_p));
	configuration.push_back(std::move(type_3_provider_p));

	auto i = injector_core{types_by_name{}, std::move(configuration)};
	QVERIFY(type_1_provider->object() == nullptr);
	QVERIFY(type_3_provider->object() == nullptr);

	auto o3 = get<type_3>(i);
	auto o1 = type_1_provider->object();
	QVERIFY(o3 != nullptr);
	QVERIFY(o1 != nullptr);
	QVERIFY(o1 == get<type_1>(i));
	QVERIFY(o1 == get<type_1_subtype_1>(i));
}

void injector_core_test::should_not_accept_ambiguous_required_supertype()
{
	auto configuration = std::vector<std::unique_ptr<provider>>{};
	configuration.push_back(make_mocked_provider<type_3, type_1>());
	configuration.push_back(make_mocked_provider<type_1_subtype_1>());
	configuration.push_back(make_mocked_provider<type_1_subtype_2>());

	expect<exception::unavailable_required_types>({"type_1", "type_3"}, [&](){
		auto i = injector_core{types_by_name{}, std::move(configuration)};
	});
}

void injector_core_test::should_accept_cyclic_dependencies()
{
	auto type_4_provider_p = make_mocked_provider<type_4>();
	auto type_4_provider = type_4_provider_p.get();
	auto type_5_provider_p = make_mocked_provider<type_5>();
	auto type_5_provider = type_5_provider_p.get();
	auto type_6_provider_p = make_mocked_provider<type_6>();
	auto type_6_provider = type_6_provider_p.get();

	auto configuration = std::vector<std::unique_ptr<provider>>{};
	configuration.push_back(std::move(type_4_provider_p));
	configuration.push_back(std::move(type_5_provider_p));
	configuration.push_back(std::move(type_6_provider_p));

	auto i = injector_core{types_by_name{make_type<type_4>(), make_type<type_5>(), make_type<type_6>()}, std::move(configuration)};
	QVERIFY(type_4_provider->object() == nullptr);
	QVERIFY(type_5_provider->object() == nullptr);
	QVERIFY(type_6_provider->object() == nullptr);

	auto o5 = get<type_5>(i);
	auto o4 = type_4_provider->object();
	auto o6 = type_6_provider->object();
	QVERIFY(o4 != nullptr);
	QVERIFY(o5 != nullptr);
	QVERIFY(o6 != nullptr);
	QVERIFY(o4 == get<type_4>(i));
	QVERIFY(o6 == get<type_6>(i));
	QVERIFY(o4 == get<type_6>(i)->o);
	QVERIFY(o5 == get<type_4>(i)->o);
	QVERIFY(o6 == get<type_5>(i)->o);
}

void injector_core_test::should_accept_dependencies_that_are_required()
{
	auto type_7_provider_p = make_mocked_provider<type_7>();
	auto type_7_provider = type_7_provider_p.get();
	auto type_8_provider_p = make_mocked_provider<type_8, type_7>();
	auto type_8_provider = type_8_provider_p.get();
	auto type_9_provider_p = make_mocked_provider<type_9>();
	auto type_9_provider = type_8_provider_p.get();

	auto configuration = std::vector<std::unique_ptr<provider>>{};
	configuration.push_back(std::move(type_7_provider_p));
	configuration.push_back(std::move(type_8_provider_p));
	configuration.push_back(std::move(type_9_provider_p));

	auto i = injector_core{types_by_name{make_type<type_7>(), make_type<type_8>(), make_type<type_9>()}, std::move(configuration)};
	QVERIFY(type_7_provider->object() == nullptr);
	QVERIFY(type_8_provider->object() == nullptr);
	QVERIFY(type_9_provider->object() == nullptr);

	auto o9 = get<type_9>(i);
	auto o7 = type_7_provider->object();
	auto o8 = type_8_provider->object();
	QVERIFY(o9 != nullptr);
	QVERIFY(o7 != nullptr);
	QVERIFY(o8 != nullptr);
	QVERIFY(o7 == get<type_7>(i));
	QVERIFY(o8 == get<type_8>(i));
}

void injector_core_test::should_inject_into_unregistered_type()
{
	auto type_1_provider_p = make_mocked_provider<type_1>();
	auto type_2_provider_p = make_mocked_provider<type_2>();

	auto configuration = std::vector<std::unique_ptr<provider>>{};
	configuration.push_back(std::move(type_1_provider_p));
	configuration.push_back(std::move(type_2_provider_p));

	auto i = injector_core{types_by_name{make_type<type_1>(), make_type<type_2>()}, std::move(configuration)};
	auto unregistered = std::unique_ptr<unregistered_type>(new unregistered_type{});
	i.inject_into(unregistered.get());

	QVERIFY(unregistered->o1 != nullptr);
	QVERIFY(unregistered->o2 != nullptr);
	QCOMPARE(unregistered->o1, get<type_1>(i));
	QCOMPARE(unregistered->o2, get<type_2>(i));
}

void injector_core_test::should_not_inject_into_when_unknown_dependencies()
{
	auto type_1_provider_p = make_mocked_provider<type_1>();

	auto configuration = std::vector<std::unique_ptr<provider>>{};
	configuration.push_back(std::move(type_1_provider_p));

	auto i = injector_core{types_by_name{make_type<type_1>()}, std::move(configuration)};
	auto unregistered = std::unique_ptr<unregistered_type>(new unregistered_type{});

	expect<exception::invalid_setter>({"type_2"}, [&](){
		i.inject_into(unregistered.get());
	});
}

// TODO: https://github.com/vogel/injeqt/issues/3
/*
void injector_core_test::should_not_accept_cyclic_required_types()
{
	auto configuration = std::vector<std::unique_ptr<provider>>{};
	configuration.push_back(make_mocked_provider<type_1, type_7>());
	configuration.push_back(make_mocked_provider<type_7, type_8>());
	configuration.push_back(make_mocked_provider<type_8, type_1>());

	expect<exception::exception>([&](){
		auto i = injector_core{std::move(configuration)};
	});
}

void injector_core_test::should_not_accept_dependent_as_required_type()
{
	auto configuration = std::vector<std::unique_ptr<provider>>{};
	configuration.push_back(make_mocked_provider<type_2>());
	configuration.push_back(make_mocked_provider<type_1, type_2>());

	expect<exception::exception>([&](){
		auto i = injector_core{std::move(configuration)};
	});
}
*/

QTEST_APPLESS_MAIN(injector_core_test)
#include "injector-core-test.moc"
