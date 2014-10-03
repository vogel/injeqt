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
#include "exception/default-constructor-not-found-exception.cpp"
#include "exception/empty-type-exception.cpp"
#include "exception/exception.cpp"
#include "exception/interface-not-implemented-exception.cpp"
#include "exception/invalid-qobject-exception.cpp"
#include "exception/invalid-setter-exception.cpp"
#include "exception/qobject-type-exception.cpp"
#include "exception/unique-factory-method-not-found-exception.cpp"
#include "exception/unresolvable-dependencies-exception.cpp"
#include "dependencies.cpp"
#include "dependency.cpp"
#include "default-constructor-method.cpp"
#include "extract-interfaces.cpp"
#include "factory-method.cpp"
#include "implementation.cpp"
#include "implemented-by.cpp"
#include "injector-impl.cpp"
#include "module.cpp"
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

#include <QtTest/QtTest>
#include <memory>

using namespace injeqt::v1;
using namespace injeqt::internal;

class by_factory_type : public QObject
{
	Q_OBJECT

public:
	Q_INVOKABLE by_factory_type() {}

};

class factory_type : public QObject
{
	Q_OBJECT

public:
	Q_INVOKABLE factory_type() {}
	Q_INVOKABLE by_factory_type * create() { return new by_factory_type{}; }

};

class factory_module : public module
{

public:
	factory_module() { add_type<factory_type>(); }
	virtual ~factory_module() {}

};

class provider_by_factory_test : public QObject
{
	Q_OBJECT

private slots:
	void should_return_always_the_same_object();

};

void provider_by_factory_test::should_return_always_the_same_object()
{
	// should be mocked
	auto modules = std::vector<std::unique_ptr<::injeqt::v1::module>>{};
	modules.emplace_back(std::unique_ptr<module>{new factory_module()});
	auto injector = injector_impl{std::move(modules)};

	auto fm = make_factory_method(make_type<by_factory_type>(), make_type<factory_type>());
	auto p = provider_by_factory{fm};

	QCOMPARE(p.provided_type(), make_type<by_factory_type>());
	QCOMPARE(p.required_types(), types{make_type<factory_type>()});
	QCOMPARE(p.factory(), fm);

	auto o = p.provide(injector);
	QCOMPARE(p.provide(injector), o);
	QCOMPARE(p.provide(injector), o);
	QCOMPARE(o->metaObject(), &by_factory_type::staticMetaObject);
}

QTEST_APPLESS_MAIN(provider_by_factory_test)
#include "provider-by-factory-test.moc"
