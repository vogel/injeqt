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

#include "exception/ambiguous-types-exception.cpp"
#include "exception/dependency-duplicated-exception.cpp"
#include "exception/dependency-on-self-exception.cpp"
#include "exception/dependency-on-subtype-exception.cpp"
#include "exception/dependency-on-supertype-exception.cpp"
#include "exception/empty-type-exception.cpp"
#include "exception/exception.cpp"
#include "exception/instantiation-failed-exception.cpp"
#include "exception/interface-not-implemented-exception.cpp"
#include "exception/invalid-dependency-exception.cpp"
#include "exception/invalid-qobject-exception.cpp"
#include "exception/invalid-setter-exception.cpp"
#include "exception/qobject-type-exception.cpp"
#include "exception/unknown-type-exception.cpp"
#include "exception/unresolvable-dependencies-exception.cpp"
#include "default-constructor-method.cpp"
#include "dependencies.cpp"
#include "dependency.cpp"
#include "factory-method.cpp"
#include "implemented-by.cpp"
#include "implementation.cpp"
#include "injector-impl.cpp"
#include "interfaces-utils.cpp"
#include "module-impl.cpp"
#include "provider-by-default-constructor.cpp"
#include "provider-by-factory.cpp"
#include "provider-ready.cpp"
#include "required-to-instantiate.cpp"
#include "resolve-dependencies.cpp"
#include "resolved-dependency.cpp"
#include "setter-method.cpp"
#include "type.cpp"
#include "type-dependencies.cpp"
#include "type-relations.cpp"
#include "types-model.cpp"

#include "expect.h"
#include "utils.h"

#include <QtTest/QtTest>
#include <string>

using namespace injeqt::internal;
using namespace injeqt::v1;

class type_1 : public QObject
{
	Q_OBJECT

public:
	Q_INVOKABLE type_1() {}

};

class factory_type_1 : public QObject
{
	Q_OBJECT

public:
	Q_INVOKABLE type_1 * create_type_1() { return nullptr; }

};

class module_impl_test : public QObject
{
	Q_OBJECT

private slots:
	void should_properly_add_providers();

};

void module_impl_test::should_properly_add_providers()
{
	auto m = module_impl{};
	QCOMPARE(m.providers().size(), 0UL);

	auto c = make_default_constructor_method(make_type<type_1>());
	auto p1 = std::unique_ptr<provider_by_default_constructor>(new provider_by_default_constructor{c});
	auto p1_ptr = p1.get();
	m.add_provider(std::move(p1));
	QCOMPARE(m.providers().size(), 1UL);
	QCOMPARE(m.providers()[0].get(), p1_ptr);

	auto fm = make_factory_method(make_type<type_1>(), make_type<factory_type_1>());
	auto p2 = std::unique_ptr<provider_by_factory>(new provider_by_factory{fm});
	auto p2_ptr = p2.get();
	m.add_provider(std::move(p2));
	QCOMPARE(m.providers().size(), 2UL);
	QCOMPARE(m.providers()[0].get(), p1_ptr);
	QCOMPARE(m.providers()[1].get(), p2_ptr);

	auto o = std::unique_ptr<type_1>(new type_1{});
	auto i = make_implementation(make_type<type_1>(), o.get());
	auto p3 = std::unique_ptr<provider_ready>(new provider_ready(i));
	auto p3_ptr = p3.get();
	m.add_provider(std::move(p3));
	QCOMPARE(m.providers().size(), 3UL);
	QCOMPARE(m.providers()[0].get(), p1_ptr);
	QCOMPARE(m.providers()[1].get(), p2_ptr);
	QCOMPARE(m.providers()[2].get(), p3_ptr);
}

QTEST_APPLESS_MAIN(module_impl_test)
#include "module-impl-test.moc"
