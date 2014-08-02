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

#include "extract-interfaces.cpp"
#include "implementation.cpp"
#include "resolved-dependency.cpp"
#include "setter-method.cpp"
#include "type.cpp"

#include "expect.h"
#include "utils.h"

#include <QtTest/QtTest>

using namespace injeqt::v1;

class type_1 : public QObject
{
	Q_OBJECT
};

class type_1_subtype_1 : public QObject
{
	Q_OBJECT
};

class type_2 : public QObject
{
	Q_OBJECT
};

class injected_type : public QObject
{
	Q_OBJECT

public:
	type_1 *_1 = nullptr;
	type_2 *_2 = nullptr;

public slots:
	injeqt_setter void setter_1(type_1 *a) { _1 = a; }
	injeqt_setter void setter_1_subtype_1(type_1_subtype_1 *) {}
	injeqt_setter void setter_2(type_2 *a) { _2 = a; }

};

class resolved_dependency_test : public QObject
{
	Q_OBJECT

public:
	resolved_dependency_test();

private slots:
	void should_throw_when_non_matching_setter_provided();
	void should_throw_when_subclass_implementation_provided();
	void should_throw_when_superclass_implementation_provided();
	void should_throw_when_applying_on_wrong_object();
	void should_properly_apply_on_valid_object();

private:
	type type_1_type;
	type type_1_subtype_1_type;
	type type_2_type;
	type injected_type_type;
	setter_method setter_1_method;
	setter_method setter_1_subtype_1_method;
	setter_method setter_2_method;

};

resolved_dependency_test::resolved_dependency_test() :
	type_1_type{make_type<type_1>()},
	type_1_subtype_1_type{make_type<type_1_subtype_1>()},
	type_2_type{make_type<type_2>()},
	injected_type_type{make_type<injected_type>()},
	setter_1_method{make_setter_method<injected_type>("setter_1(type_1*)")},
	setter_1_subtype_1_method{make_setter_method<injected_type>("setter_1_subtype_1(type_1_subtype_1*)")},
	setter_2_method{make_setter_method<injected_type>("setter_2(type_2*)")}
{
}

void resolved_dependency_test::should_throw_when_non_matching_setter_provided()
{
	auto object = make_object<type_1>();
	expect<non_matching_setter_exception>([&]{
		auto resolved = resolved_dependency{implementation{type_1_type, object.get()}, setter_2_method};
	});
}

void resolved_dependency_test::should_throw_when_subclass_implementation_provided()
{
	auto object = make_object<type_1_subtype_1>();
	expect<non_matching_setter_exception>([&]{
		auto resolved = resolved_dependency{implementation{type_1_subtype_1_type, object.get()}, setter_1_method};
	});
}

void resolved_dependency_test::should_throw_when_superclass_implementation_provided()
{
	auto object = make_object<type_1>();
	expect<non_matching_setter_exception>([&]{
		auto resolved = resolved_dependency{implementation{type_1_type, object.get()}, setter_1_subtype_1_method};
	});
}

void resolved_dependency_test::should_throw_when_applying_on_wrong_object()
{
	auto object_1 = make_object<type_1>();
	auto object_2 = make_object<type_2>();
	auto resolved = resolved_dependency{implementation{type_1_type, object_1.get()}, setter_1_method};
	expect<inavalid_apply_on_object_exception>([&]{
		resolved.apply_on(object_2.get());
	});
}

void resolved_dependency_test::should_properly_apply_on_valid_object()
{
	auto object_1 = make_object<type_1>();
	auto object_2 = make_object<type_2>();
	auto apply_on_object = make_object<injected_type>();
	auto resolved_1 = resolved_dependency{implementation{type_1_type, object_1.get()}, setter_1_method};
	auto resolved_2 = resolved_dependency{implementation{type_2_type, object_2.get()}, setter_2_method};

	QCOMPARE(static_cast<QObject *>(nullptr), static_cast<injected_type *>(apply_on_object.get())->_1);
	QCOMPARE(static_cast<QObject *>(nullptr), static_cast<injected_type *>(apply_on_object.get())->_2);

	resolved_1.apply_on(apply_on_object.get());

	QCOMPARE(object_1.get(), static_cast<injected_type *>(apply_on_object.get())->_1);
	QCOMPARE(static_cast<QObject *>(nullptr), static_cast<injected_type *>(apply_on_object.get())->_2);

	resolved_2.apply_on(apply_on_object.get());

	QCOMPARE(object_1.get(), static_cast<injected_type *>(apply_on_object.get())->_1);
	QCOMPARE(object_2.get(), static_cast<injected_type *>(apply_on_object.get())->_2);
}

QTEST_APPLESS_MAIN(resolved_dependency_test)
#include "resolved-dependency-test.moc"
