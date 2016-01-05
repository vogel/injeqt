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

#include "expect.h"
#include "utils.h"

#include <injeqt/exception/invalid-action.h>

#include "internal/action-method.h"

#include <QtTest/QtTest>

#ifndef Q_MOC_RUN
#  define INVALID_ACTION_TAG
#endif

using namespace injeqt::internal;
using namespace injeqt::v1;

class test_type : public QObject
{
	Q_OBJECT

public:
	int v = 0;

	test_type() {}

	Q_INVOKABLE test_type(int) {}

	Q_INVOKABLE INJEQT_INIT void tagged_init_action_method() { }
	Q_INVOKABLE INJEQT_DONE void tagged_done_action_method() { }

public slots:
	INJEQT_INIT void tagged_init_action_slot() { v = 1; }
	INJEQT_DONE void tagged_done_action_slot() { }
	INJEQT_INIT void invalid_init_action_arguments(int) { }
	INJEQT_DONE void invalid_done_action_arguments(int) { }
	INVALID_ACTION_TAG void invalid_action_invalid_tag() { }
	void invalid_action_no_tag() { }

signals:
	INJEQT_INIT void invalid_init_action_signal();
	INJEQT_DONE void invalid_done_action_signal();

};

class action_method_test : public QObject
{
	Q_OBJECT

private slots:
	void should_create_empty();
	void should_create_valid_from_tagged_action_method();
	void should_create_valid_from_tagged_action_slot();
	void should_invoke_have_results();
	void should_throw_when_empty_method();
	void should_throw_when_arguments();
	void should_throw_when_invalid_tag();
	void should_throw_when_no_tag();
	void should_throw_when_signal();
	void should_throw_when_constructor();

};

void action_method_test::should_create_empty()
{
	auto action = action_method{};
	QVERIFY(action.is_empty());
}

void action_method_test::should_create_valid_from_tagged_action_method()
{
	auto action = make_action_method(get_method<test_type>("tagged_init_action_method()"));
	QVERIFY(!action.is_empty());
	QCOMPARE(action.object_type(), make_type<test_type>());

	;action = make_action_method(get_method<test_type>("tagged_done_action_method()"));
	QVERIFY(!action.is_empty());
	QCOMPARE(action.object_type(), make_type<test_type>());
}

void action_method_test::should_create_valid_from_tagged_action_slot()
{
	auto action = make_action_method(get_method<test_type>("tagged_init_action_slot()"));
	QVERIFY(!action.is_empty());
	QCOMPARE(action.object_type(), make_type<test_type>());

	make_action_method(get_method<test_type>("tagged_done_action_slot()"));
	QVERIFY(!action.is_empty());
	QCOMPARE(action.object_type(), make_type<test_type>());
}

void action_method_test::should_invoke_have_results()
{
	auto action = make_action_method(get_method<test_type>("tagged_init_action_slot()"));
	auto on = make_object<test_type>();
	QCOMPARE(0, static_cast<test_type *>(on.get())->v);

	action.invoke(on.get());
	QCOMPARE(1, static_cast<test_type *>(on.get())->v);
}

void action_method_test::should_throw_when_empty_method()
{
	expect<exception::invalid_action>({"action does not have enclosing meta object"}, [&]{
		action_method{QMetaMethod{}};
	});
	expect<exception::invalid_action>({"action does not have enclosing meta object"}, [&]{
		make_action_method(QMetaMethod{});
	});
}

void action_method_test::should_throw_when_arguments()
{
	expect<exception::invalid_action>({"invalid parameter count"}, [&]{
		action_method{get_method<test_type>("invalid_init_action_arguments(int)")};
	});
	expect<exception::invalid_action>({"invalid parameter count"}, [&]{
		action_method{get_method<test_type>("invalid_done_action_arguments(int)")};
	});
	expect<exception::invalid_action>({"invalid parameter count"}, [&]{
		make_action_method(get_method<test_type>("invalid_init_action_arguments(int)"));
	});
	expect<exception::invalid_action>({"invalid parameter count"}, [&]{
		make_action_method(get_method<test_type>("invalid_done_action_arguments(int)"));
	});
}

void action_method_test::should_throw_when_invalid_tag()
{
	expect<exception::invalid_action>({"action does not have valid tag"}, [&]{
		action_method{get_method<test_type>("invalid_action_invalid_tag()")};
	});
	expect<exception::invalid_action>({"action does not have valid tag"}, [&]{
		make_action_method(get_method<test_type>("invalid_action_invalid_tag()"));
	});
}

void action_method_test::should_throw_when_no_tag()
{
	expect<exception::invalid_action>({"action does not have valid tag"}, [&]{
		action_method{get_method<test_type>("invalid_action_no_tag()")};
	});
	expect<exception::invalid_action>({"action does not have valid tag"}, [&]{
		make_action_method(get_method<test_type>("invalid_action_no_tag()"));
	});
}

void action_method_test::should_throw_when_signal()
{
	expect<exception::invalid_action>({"action is signal"}, [&]{
		action_method{get_method<test_type>("invalid_init_action_signal()")};
	});
	expect<exception::invalid_action>({"action is signal"}, [&]{
		action_method{get_method<test_type>("invalid_done_action_signal()")};
	});
	expect<exception::invalid_action>({"action is signal"}, [&]{
		make_action_method(get_method<test_type>("invalid_init_action_signal()"));
	});
	expect<exception::invalid_action>({"action is signal"}, [&]{
		make_action_method(get_method<test_type>("invalid_done_action_signal()"));
	});
}

void action_method_test::should_throw_when_constructor()
{
	expect<exception::invalid_action>({"action is constructor"}, [&]{
		action_method{get_constructor<test_type>("test_type(int)")};
	});
	expect<exception::invalid_action>({"action is constructor"}, [&]{
		make_action_method(get_constructor<test_type>("test_type(int)"));
	});
}

QTEST_APPLESS_MAIN(action_method_test)
#include "action-method-test.moc"
