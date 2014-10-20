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
#include "exception/unknown-type.cpp"
#include "exception/unique-factory-method-not-found.cpp"
#include "exception/unresolvable-dependencies.cpp"
#include "dependencies.cpp"
#include "dependency.cpp"
#include "implementation.cpp"
#include "implemented-by.cpp"
#include "injector-core.cpp"
#include "interfaces-utils.cpp"
#include "required-to-instantiate.cpp"
#include "resolved-dependency.cpp"
#include "resolve-dependencies.cpp"
#include "setter-method.cpp"
#include "type.cpp"
#include "type-dependencies.cpp"
#include "type-relations.cpp"
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

class injector_core_test : public QObject
{
	Q_OBJECT

private slots:
	void should_create_empty_injector_core();
	void should_create_simple_injector_core_and_return_object();
	void should_not_accept_doubled_type();

};

void injector_core_test::should_create_empty_injector_core()
{
	auto i = injector_core{};
	expect<exception::unknown_type>([&](){
		i.get(make_type<type_1>());
	});
}

void injector_core_test::should_create_simple_injector_core_and_return_object()
{
	auto type_1_provider_p = make_mocked_provider<type_1>();
	auto type_1_provider = type_1_provider_p.get();
	auto configuration = std::vector<std::unique_ptr<provider>>{};
	configuration.push_back(std::move(type_1_provider_p));

	auto i = injector_core{std::move(configuration)};
	QVERIFY(type_1_provider->object() == nullptr);

	auto o1 = qobject_cast<type_1 *>(i.get(make_type<type_1>()));
	auto o2 = qobject_cast<type_1 *>(i.get(make_type<type_1>()));

	QVERIFY(o1 != nullptr);
	QVERIFY(o1 == o2);
	QVERIFY(type_1_provider->object() == o1);
}

void injector_core_test::should_not_accept_doubled_type()
{
	auto configuration = std::vector<std::unique_ptr<provider>>{};
	configuration.push_back(make_mocked_provider<type_1>());
	configuration.push_back(make_mocked_provider<type_1>());

	expect<exception::ambiguous_types>([&](){
		auto i = injector_core{std::move(configuration)};
	});
}

QTEST_APPLESS_MAIN(injector_core_test)
#include "injector-core-test.moc"
