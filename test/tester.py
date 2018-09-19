#!/usr/bin/env python
import os
import re
import sys

import pexpect

SHELL = "./w4118_sh"


class TesterPrinter(object):

    @staticmethod
    def stderr(msg):
        print >> sys.stderr, msg

    @staticmethod
    def stdout(msg):
        print msg

    def results(self, results):
        failed_count = 0

        for r in results:
            if not r.failed:
                continue

            failed_count += 1

            self.stdout("Test failed ('%s')!" % r.test.cmd)
            self.stdout("\tExpected: '%s'" % r.test.expected)
            self.stdout("\tGot: '%s'" % r.got)

        test_count = len(results)
        self.stdout("%d of %d tests passed" %
                    (test_count - failed_count, test_count))


class TestCase(object):

    def __init__(self, cmd=None, expected=None):
        self.cmd = cmd
        self.expected = expected


class TestResult(object):

    def __init__(self, test, failed, got):
        self.test = test
        self.failed = failed
        self.got = got


class HomeworkTester(object):
    SHELL_PROMPT = re.escape("$")
    PATH_DELIM = "[:;]"

    def __init__(self, printer=TesterPrinter(), timeout=2):
        self.timeout = timeout
        self.printer = printer
        self.results = []
        self.tests = []
        self.p = None

    def _new_shell(self):
        p = pexpect.spawn(SHELL, timeout=self.timeout)
        p.setecho(False)

        try:
            ret = p.expect([self.SHELL_PROMPT, pexpect.EOF])
        except pexpect.TIMEOUT:
            self.printer.stderr(
                "fatal: cannot match prompt text. cannot proceed!")
            sys.exit(1)
        if ret == 1:
            self.printer.stdout("Program exits without any prompt $")
            sys.exit(1)

        self.p = p

    def _close_shell(self):
        assert self.p, "Attempted to close a non-existent shell."

        if self.p.isalive():
            os.killpg(self.p.pid, 9)
            self.p.kill(9)

        self.p.close()

    def _execute_command(self, test_case):
        """Check status of shell, then send command.

        Returns a TestResult representing whether the command executed,
        but not whether the output of the command was successful
        """
        assert type(test_case) is TestCase, \
            "Cannot run test on something that isn't a TestCase: %s" % test_case
        assert self.p, "Cannot run test unless a shell is running. Use run()."

        if not self.p.isalive():
            # The shell died
            result = TestResult(test_case, True, "CRASH!")
            return result

        # Try to execute the test case's command
        self.p.sendline(test_case.cmd)

        try:
            self.p.expect([self.SHELL_PROMPT, pexpect.EOF])
        except pexpect.TIMEOUT:
            # The shell probably froze
            result = TestResult(test_case, True, "HANG!")
            return result

        got = self.p.before.replace('\r', '').rstrip('\n').strip()
        return TestResult(test_case, False, got)

    def _analyze_path_result(self, expected, got):
        paths = set(re.split(self.PATH_DELIM, got))
        expected_paths = set(expected.split(':'))

        path_success = (paths.symmetric_difference(expected_paths) == set([]))

        return path_success

    def _analyze_result(self, t, result):
        if t.cmd == "path":
            if self._analyze_path_result(t.expected, result.got):
                return TestResult(t, False, result.got)
            else:
                return TestResult(t, True, result.got)
        else:
            failed = False

            if type(t.expected) is str and t.expected != "":
                failed = (re.match("^{}$".format(t.expected), result.got) is None)
            else:
                failed = (t.expected != result.got)

            return TestResult(t, failed, result.got)

    def add_test(self, cmd, expected):
        """Add a command with an expected output to the
        list of tests to execute.

        @cmd - A string representing the exact command to execute
        @expected - A regex string
        """
        self.tests.append(TestCase(cmd, expected))

    def reset(self):
        """Closes shell, and empties the tests and results lists."""
        self._close_shell()
        self.tests = []
        self.results = []

    def run(self):
        """Creates new shell and runs tests, returning the results list."""
        path_backup = os.environ['PATH']
        os.environ['PATH'] = ''

        self._new_shell()

        for t in self.tests:
            result = self._execute_command(t)

            # If result was not a failure, we can continue tests
            if not result.failed:
                result = self._analyze_result(t, result)

            self.results.append(result)

        os.environ['PATH'] = path_backup

        return self.results

    def print_results(self):
        self.printer.results(self.results)


if __name__ == "__main__":
    import homework_tests
