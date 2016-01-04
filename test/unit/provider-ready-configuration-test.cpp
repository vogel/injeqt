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
#include "exception/dependency-duplicated.cpp"
#include "exception/dependency-on-self.cpp"
#include "exception/dependency-on-subtype.cpp"
#include "exception/dependency-on-supertype.cpp"
#include "exception/empty-type.cpp"
#include "exception/exception.cpp"
#include "exception/instantiation-failed.cpp"
#include "exception/interface-not-implemented.cpp"
#include "exception/invalid-action.cpp"
#include "exception/invalid-dependency.cpp"
#include "exception/invalid-qobject.cpp"
#include "exception/invalid-setter.cpp"
#include "exception/qobject-type.cpp"
#include "exception/unavailable-required-types.cpp"
#include "exception/unknown-type.cpp"
#include "exception/unresolvable-dependencies.cpp"
#include "action-method.cpp"
#include "dependencies.cpp"
#include "dependency.cpp"
#include "implementation.cpp"
#include "implemented-by.cpp"
#include "injector-core.cpp"
#include "interfaces-utils.cpp"
#include "provider-ready.cpp"
#include "provider-ready-configuration.cpp"
#include "required-to-satisfy.cpp"
#include "resolve-dependencies.cpp"
#include "resolved-dependency.cpp"
#include "setter-method.cpp"
#include "type.cpp"
#include "type-dependencies.cpp"
#include "type-relations.cpp"
#include "types-by-name.cpp"
#include "types-model.cpp"

#include "expect.h"
#include "utils.h"

#include <QtTest/QtTest>
#include <memory>

using namespace injeqt::v1;
using namespace injeqt::internal;

class type_1 : public QObject
{
	Q_OBJECT
};

class type_2 : public QObject
{
	Q_OBJECT
};

class provider_ready_configuration_test : public QObject
{
	Q_OBJECT

private slots:
	void should_accept_qobject_type_and_throw_on_create();
	void should_accept_nullptr_object_and_throw_on_create();
	void should_accept_not_implemented_interface_and_throw_on_create();
	void should_accept_valid_configuration();

};

void provider_ready_configuration_test::should_accept_qobject_type_and_throw_on_create()
{
	auto pc = provider_ready_configuration{make_type<QObject>(), nullptr};

	expect<exception::qobject_type>([&](){
		pc.create_provider({});
	});
}

void provider_ready_configuration_test::should_accept_nullptr_object_and_throw_on_create()
{
	auto pc = provider_ready_configuration{make_type<type_1>(), nullptr};

	expect<exception::invalid_qobject>([&](){
		pc.create_provider({});
	});
}

void provider_ready_configuration_test::should_accept_not_implemented_interface_and_throw_on_create()
{
	auto o = make_object<type_2>();
	auto pc = provider_ready_configuration{make_type<type_1>(), o.get()};

	expect<exception::interface_not_implemented>([&](){
		pc.create_provider({});
	});
}

void provider_ready_configuration_test::should_accept_valid_configuration()
{
	auto i = injector_core{};
	auto o = make_object<type_1>();
	auto pc = provider_ready_configuration{make_type<type_1>(), o.get()};
	auto p = pc.create_provider({});

	QCOMPARE(o.get(), p->provide(i));
}

QTEST_APPLESS_MAIN(provider_ready_configuration_test)
#include "provider-ready-configuration-test.moc"
