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
#include "default-constructor-method.cpp"
#include "dependencies.cpp"
#include "dependency.cpp"
#include "factory-method.cpp"
#include "implementation.cpp"
#include "implemented-by.cpp"
#include "injector.cpp"
#include "injector-core.cpp"
#include "injector-impl.cpp"
#include "interfaces-utils.cpp"
#include "module.cpp"
#include "module-impl.cpp"
#include "provider-by-default-constructor.cpp"
#include "provider-by-default-constructor-configuration.cpp"
#include "provider-by-factory.cpp"
#include "provider-by-factory-configuration.cpp"
#include "provider-ready.cpp"
#include "provider-ready-configuration.cpp"
#include "required-to-instantiate.cpp"
#include "resolved-dependency.cpp"
#include "resolve-dependencies.cpp"
#include "setter-method.cpp"
#include "type.cpp"
#include "type-dependencies.cpp"
#include "type-relations.cpp"
#include "types-by-name.cpp"
#include "types-model.cpp"

#include "expect.h"
#include "utils.h"

#include <QtTest/QtTest>
#include <string>

using namespace injeqt::internal;
using namespace injeqt::v1;

class created_by_factory : public QObject
{
	Q_OBJECT
public:
	created_by_factory() {}
};

class default_constructible_factory : public QObject
{
	Q_OBJECT
public:
	Q_INVOKABLE default_constructible_factory() {}
	Q_INVOKABLE created_by_factory * create() { return new created_by_factory{}; }
};

class ready_type : public QObject
{
	Q_OBJECT
};

class test_module : public module
{
public:
	test_module()
	{
		_ready_object = std::unique_ptr<ready_type>{new ready_type{}};

		add_type<default_constructible_factory>();
		add_factory<created_by_factory, default_constructible_factory>();
		add_ready_object<ready_type>(_ready_object.get());
	}
private:
	std::unique_ptr<ready_type> _ready_object;
};

class injector_test : public QObject
{
	Q_OBJECT

private slots:
	void should_create_empty_injector();
	void should_no_accept_qobject_type();
	void should_no_accept_unknown_type();
	void should_create_valid_injector();
	void should_allow_move();

};

class not_configured_type : public QObject
{
	Q_OBJECT
};

void injector_test::should_create_empty_injector()
{
	injector{};
}

void injector_test::should_no_accept_qobject_type()
{
	auto i = injector{};

	expect<exception::qobject_type>([&](){
		i.get<QObject>();
	});
}

void injector_test::should_no_accept_unknown_type()
{
	auto i = injector{};

	expect<exception::unknown_type>({"default_constructible_factory"}, [&](){
		i.get<default_constructible_factory>();
	});
}

void injector_test::should_create_valid_injector()
{
	auto modules = std::vector<std::unique_ptr<injeqt::module>>{};
	modules.emplace_back(std::unique_ptr<test_module>(new test_module{}));
	auto i = injector{std::move(modules)};

	QVERIFY(i.get<created_by_factory>() != nullptr);
	QVERIFY(i.get<default_constructible_factory>() != nullptr);
	QVERIFY(i.get<ready_type>() != nullptr);

	expect<exception::unknown_type>({"not_configured_type"}, [&](){
		i.get<not_configured_type>();
	});
}

void injector_test::should_allow_move()
{
	auto modules = std::vector<std::unique_ptr<injeqt::module>>{};
	modules.emplace_back(std::unique_ptr<test_module>(new test_module{}));
	auto i = injector{std::move(modules)};
	auto i2 = std::move(i);
	auto i3 = injector{};
	i3 = std::move(i2);

	QVERIFY(i3.get<created_by_factory>() != nullptr);
	QVERIFY(i3.get<default_constructible_factory>() != nullptr);
	QVERIFY(i3.get<ready_type>() != nullptr);

	expect<exception::unknown_type>({"not_configured_type"}, [&](){
		i3.get<not_configured_type>();
	});
}

QTEST_APPLESS_MAIN(injector_test)
#include "injector-test.moc"
