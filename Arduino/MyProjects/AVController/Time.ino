void updateCurrentTime() {
  time_t nextCurrentTime = now();
  if(nextCurrentTime > currentTime) {
    currentTime = nextCurrentTime;
  }
  // if time has moved backwards (I suspect this might happen when the time library synchronizes with the RTC) we just don't update the currentTime field.
  // the code should already handle multiple loops within the same second but moving time backwords is not expeceted and not handled so we avoid this.
}

void initTime() {
#ifdef debugRtc
  tmElements_t tm;
#endif
  time_t t;
  bool enterNewTime = false;

  if (RTC.haltRTC()) {
#ifdef debugRtc
    pl("The DS1302 is stopped. It needs to be initialized with time.");
#endif
    enterNewTime = true;
  } else {
    t = RTC.get();
    if (year(t) < 2018 || year(t) > 2030) {
#ifdef debugRtc
      p("Time out of range: ");
      p(t);
      pl(" Epoch/UTC");
#endif
      fatalError();
    }
#ifdef debugRtc

    p("Current time is ");
    p(t);
    p(" Epoch/UTC (");
    p(epochToLocal(t));
    pl(" Local)");
    pl("Send any letter now to change the time.");
    delay(10000);
    enterNewTime = Serial.read() > 0;
#endif
  }

  if (enterNewTime) {
#ifdef debugRtc
    tm.Year = y2kYearToTm(read2digits("Enter year (2 digits, 18 to 28):", 18, 28));
    tm.Month = read2digits("Enter month (1 to 12):", 1, 12);
    tm.Day = read2digits("Enter day (1 to 31):", 1, 31);
    tm.Hour = read2digits("Enter hour (0 to 23):", 0, 23);
    tm.Minute = read2digits("Enter minute (0 to 59):", 0, 59);
    tm.Second = 0;

    t = localToEpoch(makeTime(tm));

    if (RTC.set(t) == 0) {
      setTime(t);
      p("Time set to: ");
      p(epochToLocal(t));
      pl("");
    } else {
      pl("Error setting time.");
      fatalError();
    }
#else
    fatalError();
#endif
  }
  //setSyncProvider() causes the Time library to synchronize with the
  //external RTC by calling RTC.get() every five minutes by default.
  setSyncProvider(RTC.get);

  if (timeStatus() == timeSet) {
#ifdef debugRtc
    pl("Time library synchronized with RTC");
#endif
  } else {
#ifdef debugRtc
    pl("Time library could not synchronize with RTC");
#endif
    fatalError();
  }
  updateCurrentTime();
}

#ifdef debugRtc
int read2digits(const char *s, int min, int max) {
  int i = -1;
  do {
    if (i != -1)
      pl("Out of range");
    pl(s);
    while (Serial.available() < 1);
    delay(500);
    i = Serial.parseInt();
    p("Received ");
    pl(i);
  } while (i < min || i > max);
  return i;
}
#endif

bool isLocalDaylightSavingsTime(time_t t) {
  tmElements_t tmStart, tmEnd;
  time_t tStart, tEnd;
  int calendarStartYear = 2025;
  int calendarEndYear = 2035;
  int y = year(t);
  if (y < calendarStartYear || y > calendarEndYear)
  {
    fatalError();
  }
  // day in month for daylight savings period for the years from calendarStartYear to calendarEndYear
  int startDays[] = { 30, 29, 28, 26, 25, 31, 30, 28, 27, 26, 25 };
  int endDays[] =   { 26, 26, 31, 29, 28, 27, 26, 31, 30, 29, 28 };
  tmStart.Year = CalendarYrToTm(y);
  tmStart.Month = 3;
  tmStart.Day = startDays[y - calendarStartYear];
  tmStart.Hour = 2;
  tmStart.Minute = 0;
  tmStart.Second = 0;

  tmEnd.Year = CalendarYrToTm(y);
  tmEnd.Month = 10;
  tmEnd.Day = endDays[y - calendarStartYear];
  tmEnd.Hour = 2;
  tmEnd.Minute = 0;
  tmEnd.Second = 0;

  tStart = makeTime(tmStart);
  tEnd = makeTime(tmEnd);

  return t >= tStart && t < tEnd;
}

time_t epochToLocal(time_t tEpoch) {
  time_t tLocal = tEpoch + SECS_PER_HOUR;
  if (isLocalDaylightSavingsTime(tLocal))
    tLocal = tLocal + SECS_PER_HOUR;
  return tLocal;
}

time_t localToEpoch(time_t tLocal) {
  time_t tEpoch;
  if (isLocalDaylightSavingsTime(tLocal))
    tEpoch = tLocal - 2 * SECS_PER_HOUR;
  else
    tEpoch = tLocal - SECS_PER_HOUR;
  return tEpoch;
}
