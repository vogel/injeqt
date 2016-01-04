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
#include "exception/invalid-action.cpp"
#include "exception/invalid-dependency.cpp"
#include "exception/invalid-qobject.cpp"
#include "exception/invalid-setter.cpp"
#include "exception/qobject-type.cpp"
#include "exception/unavailable-required-types.cpp"
#include "exception/unknown-type.cpp"
#include "exception/unique-factory-method-not-found.cpp"
#include "exception/unresolvable-dependencies.cpp"
#include "action-method.cpp"
#include "dependencies.cpp"
#include "dependency.cpp"
#include "default-constructor-method.cpp"
#include "factory-method.cpp"
#include "implementation.cpp"
#include "implemented-by.cpp"
#include "injector-core.cpp"
#include "interfaces-utils.cpp"
#include "provider-by-default-constructor.cpp"
#include "provider-by-factory.cpp"
#include "provider-ready.cpp"
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

class invalid_factory_type : public QObject
{
	Q_OBJECT

public:
	Q_INVOKABLE invalid_factory_type() {}
	Q_INVOKABLE by_factory_type * create() { return nullptr; }

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

public:
	provider_by_factory_test();

private slots:
	void should_return_always_the_same_object();
	void should_throw_instantiation_failed_when_invalid_factory();

private:
	types_by_name known_types;

};

provider_by_factory_test::provider_by_factory_test()
{
	known_types = types_by_name{std::vector<type>{
		make_type<by_factory_type>(),
		make_type<factory_type>(),
		make_type<invalid_factory_type>()
	}};
}

void provider_by_factory_test::should_return_always_the_same_object()
{
	auto fm = make_factory_method(known_types, make_type<by_factory_type>(), make_type<factory_type>());
	auto fp = std::unique_ptr<provider_by_factory>{new provider_by_factory{fm}};
	auto p = fp.get();

	auto configuration = std::vector<std::unique_ptr<provider>>{};
	configuration.emplace_back(std::move(fp));
	configuration.emplace_back(std::unique_ptr<provider_by_default_constructor>{new provider_by_default_constructor{make_default_constructor_method(make_type<factory_type>())}});
	auto injector = injector_core{types_by_name{}, std::move(configuration)};

	QCOMPARE(p->provided_type(), make_type<by_factory_type>());
	QCOMPARE(p->required_types(), types{make_type<factory_type>()});
	QCOMPARE(p->factory(), fm);

	auto o = p->provide(injector);
	QCOMPARE(p->provide(injector), o);
	QCOMPARE(p->provide(injector), o);
	QCOMPARE(o->metaObject(), &by_factory_type::staticMetaObject);
}

void provider_by_factory_test::should_throw_instantiation_failed_when_invalid_factory()
{
	auto fm = make_factory_method(known_types, make_type<by_factory_type>(), make_type<invalid_factory_type>());
	auto fp = std::unique_ptr<provider_by_factory>{new provider_by_factory{fm}};
	auto p = fp.get();

	auto configuration = std::vector<std::unique_ptr<provider>>{};
	configuration.emplace_back(std::move(fp));
	configuration.emplace_back(std::unique_ptr<provider_by_default_constructor>{new provider_by_default_constructor{make_default_constructor_method(make_type<invalid_factory_type>())}});
	auto injector = injector_core{types_by_name{}, std::move(configuration)};

	QCOMPARE(p->provided_type(), make_type<by_factory_type>());
	QCOMPARE(p->required_types(), types{make_type<invalid_factory_type>()});
	QCOMPARE(p->factory(), fm);

	expect<exception::instantiation_failed>({"by_factory_type"}, [&](){
		p->provide(injector);
	});
}

QTEST_APPLESS_MAIN(provider_by_factory_test)
#include "provider-by-factory-test.moc"
