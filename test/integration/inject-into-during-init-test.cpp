/*
 * %injeqt copyright begin%
 * Copyright 2015 Rafał Malinowski (rafal.przemyslaw.malinowski@gmail.com)
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

#include <injeqt/injector.h>
#include <injeqt/module.h>

#include <QtTest/QtTest>

injeqt::injector global_injector;

class injectable;

class injeqt_into_me : public QObject
{
	Q_OBJECT

public:
	injeqt_into_me() {}
	virtual ~injeqt_into_me() {}

private slots:
	INJEQT_SET void set_injectable(injectable *) {}

};

class injectable : public QObject
{
	Q_OBJECT

public:
	Q_INVOKABLE injectable() {}
	virtual ~injectable() {}

private slots:
	INJEQT_INIT void init()
	{
		injeqt_into_me o;
		global_injector.inject_into(&o);
	}

};

class inject_into_during_init_test : public QObject
{
	Q_OBJECT

private slots:
	void should_properly_inject_into_during_init();

};

void inject_into_during_init_test::should_properly_inject_into_during_init()
{
	class m : public injeqt::module
	{
	public:
		m()
		{
			add_type<injectable>();
		}
		virtual ~m() {}
	};

	auto modules = std::vector<std::unique_ptr<injeqt::module>>{};
	modules.emplace_back(std::unique_ptr<m>{new m{}});
	global_injector = injeqt::injector{std::move(modules)};
	global_injector.get<injectable>();
}

QTEST_APPLESS_MAIN(inject_into_during_init_test)
#include "inject-into-during-init-test.moc"
