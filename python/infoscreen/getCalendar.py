from ics import Calendar
from os import getenv
from requests import get
import calendar
import arrow


def todays_events():
    url = getenv('CAL_URL')
    g = get(url).text
    c = Calendar(g)
    today = arrow.utcnow()
    events = c.timeline
    result = []

    for event in events:
        if today.month == event.begin.month and  \
            today.day == event.begin.day and \
            event.end.timestamp> today.timestamp:
            result.append(event)

    return result

def print_events(events):
    for event in events:
        offset = get_UTC_offset(event.begin)
        print('==================================')
        print("{} {} - {}\n\n{}\n\n{}" .format(
            event.begin.shift(hours=offset).format('DD.MM.YYYY'), 
            event.begin.shift(hours=offset).format('HH:mm'),
            event.end.shift(hours=offset).format('HH:mm'),
            event.name,
            event.description))
        print('==================================')
        words = event.description \
            .replace("<br>", " ") \
            .replace("<span>", "") \
            .replace("</span>", "") \
            .split()
        print (words)

# Based on rosetta code example
# https://rosettacode.org/wiki/Find_the_last_Sunday_of_each_month#Python
def get_UTC_offset(datetime):
    year = datetime.year

    # Summer time in Finland begins third month last sunday
    # Summer time in Finland ends tenth month last sunday
    begin = max(week[-1] for week in calendar.monthcalendar(year, 3)), 3
    end = max(week[-1] for week in calendar.monthcalendar(year, 10)), 10

    if datetime.month == begin[1]:
        if datetime.day >= begin[0]:
            return 3
        else:
            return 2
    elif datetime.month == end[1]:
        if datetime.day < end[0]:
            return 3
        else:
            return 2
    elif datetime.month > begin[1] and datetime.month < end[1]:
        return 3
    else:
        return 2


if __name__ == "__main__":
    from dotenv import load_dotenv
    import os

    load_dotenv('.env')

    events = todays_events()
    print_events(events)
