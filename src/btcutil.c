/*
	Copyright (C) 2013  Marco Scannadinari

	This file is part of btcwatch.

	btcwatch is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	btcwatch is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with btcwatch.  If not, see <http://www.gnu.org/licenses/>.
*/

#define BOLD(str) "\033[1m" str "\033[0m"
#define IGNORE(x) (void) (x)

#include <error.h>
#include <pwd.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <wchar.h>

#include "include/config.h"
#include "include/btcutil.h"

void btcdbg(const char *const fmt, ...) {
	#ifdef DEBUG
	va_list args;

	va_start(args, fmt);

	bputs(BOLD("DEBUG: "));
	vprintf(fmt, args);
	putchar('\n');

	va_end(args);
	#else
	IGNORE(fmt);
	#endif
}

void find_path(char *const path, char *const pathwf) {
	btcdbg("find_path()");

	struct passwd *userinfo;

	userinfo = getpwuid(getuid());
	strcpy(path, userinfo -> pw_dir);
	strcat(path, "/.btcwatch");
	strcpy(pathwf, path);
	strcat(pathwf, "/btcstore");

	btcdbg("~/: %s", userinfo -> pw_dir);
	btcdbg("~/.btcwatch: %s", path);
	btcdbg("~/.btcwatch/btcstore: %s", pathwf);
}

noreturn void help(const char *const prog_nm, const char *const topic) {
	btcdbg("help()");

	char currcies[][3 + 1] = {
		#ifdef MT_GOX_API
		"AUD",
		"CAD",
		"CHF",
		"CNY",
		"CZK",
		"DKK",
		#endif
		"EUR",
		#ifdef MT_GOX_API
		"GBP",
		"HKD",
		"JPY",
		"NOK",
		"PLN",
		#endif
		#ifdef MT_GOX_API
		"RUB",
		#elif defined(BTC_E_API)
		"RUR",
		#endif
		#ifdef MT_GOX_API
		"SEK",
		"SGD",
		"THB",
		#endif
		"USD",
	};

	char topics[][16] = {
		"currencies"
	};

	/*
	help() uses argv[0] for the program name. Rationale: because of the
	"Usage:" string, where the user expects to be given an explenation on
	how to use and invoke the program. For example, if the program is
	renamed or invoked via a symbolic link, help() will still output a
	valid "Usage:" string.

	version(), on the other hand, merely displays the program's name,
	its version number, and licensing information, all of which should
	remain constant (until the next release).
	*/

	if(!topic) {
		bputs("Usage: "); bputs(prog_nm); bputs(" [OPTION]\n");
		bputs(
			"Get and monitor Bitcoin trade information\n"
			"\n"
			"Options:       Long options:\n"
			"  -C             --compare              comare current price with stored price\n"
			"  -S             --store                store current price\n"
			"  -a             --all                  equivalent to -pbs\n"
			"  -b             --buy                  print buy price\n"
			"  -c CURRENCY    --currency=CURRENCY    set conversion currency\n"
			"  -n AMOUNT      --amount=AMOUNT        sets the amount of Bitcoin to convert\n"
			"  -o             --colour, --color      enables use of colour\n"
			"  -p             --ping                 check for a successful JSON response\n"
			"  -r             --reverse              convert currency to Bitcoin\n"
			"  -s             --sell                 print sell price\n"
			"  -v             --verbose              increase verbosity\n"
			"\n"
			"  -? [topic]     --help[=topic]         print this help, or help designated by topic\n"
			"                                        use --help=topics for available topics\n"
			"  -V             --version              print version number\n"
			"\n"
			"Report bugs to " PACKAGE_BUGREPORT "\n"
			"btcwatch home page: " PACKAGE_URL "\n"
		);
		exit(EXIT_SUCCESS);
	} else {
		if(!strcmp(topic, "currencies")) {
			for(
				uint_fast8_t i = 0;
				i < (sizeof currcies / sizeof currcies[0]);
				++i
			) puts(currcies[i]);
			exit(EXIT_SUCCESS);
		} else if(!strcmp(topic, "topics")) {
			for(
				uint_fast8_t i = 0;
				i < (sizeof topics / sizeof topics[0]);
				++i
			) puts(topics[i]);
			exit(EXIT_SUCCESS);
		} else {
			error(EXIT_FAILURE, 0, "no such topic");
		}
	}
	exit(EXIT_SUCCESS);
}

void resetb(void) {
	freopen(NULL, "a", stdout);  // reopen stdout
	fwide(stdout, -1);  // set stdout to be byte-oriented

	btcdbg("resetb()");  // this goes at the end in order to be printed on a byte-oriented stream (assuming that the stream was wide-oriented before resetb()'s call)
}

void resetw(void) {
	btcdbg("resetw()");

	freopen(NULL, "a", stdout);  // ^
	fwide(stdout, 1);  // set stdout to be wide-oriented
}

noreturn void version(void) {
	btcdbg("version()");

	printf(
		"%s (%s)\n",
		PACKAGE_STRING,
		#ifdef MT_GOX_API
		"MtGox"
		#elif defined(BTC_E_API)
		"BTC-E"
		#endif
	);
	bputs(
		"Copyright (C) Marco Scannadinari.\n"
		"License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>\n"
		"This is free software: you are free to change and redistribute it.\n"
		"There is NO WARRANTY, to the extent permitted by law.\n"
		"\n"
		"Written by Marco Scannadinari.\n"
	);

	exit(EXIT_SUCCESS);
}
