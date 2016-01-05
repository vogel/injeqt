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

#include "internal/injector-core.h"
#include "internal/provider-ready.h"

#include <QtTest/QtTest>
#include <memory>

using namespace injeqt::v1;
using namespace injeqt::internal;

class ready_type : public QObject
{
	Q_OBJECT
};

class provider_ready_test : public QObject
{
	Q_OBJECT

private slots:
	void should_return_always_the_same_object();

};

void provider_ready_test::should_return_always_the_same_object()
{
	auto empty_injector1 = injector_core{};
	auto empty_injector2 = injector_core{};
	auto object = std::unique_ptr<QObject>(new ready_type());
	auto i = implementation{make_type<ready_type>(), object.get()};
	auto p = provider_ready{i};

	QCOMPARE(p.provided_type(), make_type<ready_type>());
	QCOMPARE(p.required_types(), types{});
	QCOMPARE(p.ready_implementation(), i);

	QCOMPARE(p.provide(empty_injector1), object.get());
	QCOMPARE(p.provide(empty_injector2), object.get());
}

QTEST_APPLESS_MAIN(provider_ready_test)
#include "provider-ready-test.moc"
