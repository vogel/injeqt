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
#include "exception/invalid-dependency-exception.cpp"
#include "exception/interface-not-implemented-exception.cpp"
#include "exception/invalid-qobject-exception.cpp"
#include "exception/invalid-setter-exception.cpp"
#include "exception/qobject-type-exception.cpp"
#include "exception/unknown-type-exception.cpp"
#include "exception/unresolvable-dependencies-exception.cpp"
#include "dependencies.cpp"
#include "dependency.cpp"
#include "default-constructor-method.cpp"
#include "extract-interfaces.cpp"
#include "implementation.cpp"
#include "implemented-by.cpp"
#include "injector-impl.cpp"
#include "module-impl.cpp"
#include "provider-by-default-constructor.cpp"
#include "required-to-instantiate.cpp"
#include "resolve-dependencies.cpp"
#include "resolved-dependency.cpp"
#include "setter-method.cpp"
#include "type.cpp"
#include "type-dependencies.cpp"
#include "type-relations.cpp"
#include "types-model.cpp"

#include "expect.h"

#include <QtTest/QtTest>
#include <memory>

using namespace injeqt::v1;
using namespace injeqt::internal;

class default_constructor_type : public QObject
{
	Q_OBJECT

public:
	Q_INVOKABLE default_constructor_type() {}

};

class provider_by_default_constructor_test : public QObject
{
	Q_OBJECT

private slots:
	void should_return_always_the_same_object();

};

void provider_by_default_constructor_test::should_return_always_the_same_object()
{
	auto empty_injector1 = injector_impl{};
	auto empty_injector2 = injector_impl{};
	auto c = make_default_constructor_method(make_type<default_constructor_type>());
	auto p = provider_by_default_constructor{c};

	QCOMPARE(p.provided_type(), make_type<default_constructor_type>());
	QCOMPARE(p.required_types(), types{});
	QCOMPARE(p.constructor(), c);

	auto o = p.provide(empty_injector1);
	QCOMPARE(p.provide(empty_injector1), o);
	QCOMPARE(p.provide(empty_injector2), o);
	QCOMPARE(o->metaObject(), &default_constructor_type::staticMetaObject);
}

QTEST_APPLESS_MAIN(provider_by_default_constructor_test)
#include "provider-by-default-constructor-test.moc"
