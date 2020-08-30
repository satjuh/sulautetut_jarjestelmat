from curses import wrapper, newwin
import arrow
from math import floor
from getCalendar import todays_events, get_UTC_offset
from weather import get_weather


def add_events(window, window_width, window_height):
    events = todays_events()
    center = floor(window_width / 2)
    line = 1

    if len(events) == 0:
        string = "Nothing today"
        window.addstr(1, floor(window_width/ 2) - floor(len(string)/ 2), string)
        return

    for event in events:
        if line + 3 > window_height:
            return
        offset = get_UTC_offset(event.begin)
        
        # Get time interval
        time = "| " + event.begin.shift(hours=offset).format('HH:mm') + " - " \
            + event.end.shift(hours=offset).format('HH:mm') + " " \
            + event.name + " |"

        # Add to the center of the window
        x = center - floor(len(time) / 2)

        # Add starting time in ascii art box
        if len(time) > window_width:
            window.addstr(line, 2, "=" * (window_width - 5))
            line = line + 1

            time_string = time [0:window_width -10] + "... |"
            window.addstr(line, 2, time_string)
            line = line + 1

            window.addstr(line, 2, "=" * (window_width - 5))
            line = line + 1
        else:
            window.addstr(line, x, "=" * (len(time)))
            line = line + 1

            window.addstr(line, x, time)
            line = line + 1

            window.addstr(line, x, "=" * (len(time)))
            line = line + 1

        # Add description
        if len(event.description ) > window_width:
            # Better version
            words = event.description.split(" ")
            # Line starts at 2
            current_width = 2
            for word in words:
                if line == window_height:
                    return
                if current_width + len(word) < window_width - 2:
                    window.addstr(line, current_width, word)
                    current_width = current_width + len(word) + 1
                else:
                    line = line + 1
                    current_width = 2
                    window.addstr(line, current_width, word)
                    current_width = current_width + len(word) +1


        elif len(event.description) > 0 and line + 1 < window_height:
            window.addstr(line, center - floor(len(event.description)), event.description)
            line = line + 1

        line = line + 2

        
def add_weather(window, window_width, window_height):
    weather_rows = get_weather()
    line = 1
    x = 2
    for row in weather_rows:
        if line + 4 > window_height:
            break
        hour = "| " + row.hour  + " |"
        art = "=" * len(hour)

        window.addstr(line, x, art + "  " + row.temp + ", " + row.feels.replace(" astetta", "°"))
        line = line + 1
        
        window.addstr(line, x, hour + "  " + row.cloud)
        line = line + 1

        window.addstr(line, x, art + "  " + row.wind.split(",")[0])
        line = line + 1
        line = line + 1
    return


def main(stdsrc):
    stdsrc.clear()

    rows, cols = stdsrc.getmaxyx()

    # Clock window
    format_string = "DD.MM.YYYY kello: HH:mm"
    clock_height = 3
    clock = newwin(clock_height, cols, 0, 0)
    # find the center of the clock window
    clock_x = floor(cols / 2) - floor(len(format_string)/ 2)
    clock_y = floor(clock_height / 2) 

    # Calendar window
    calendar_height = rows - 3
    calendar_width = int (cols * (1 / 2))
    calendar = newwin(calendar_height, calendar_width, clock_height, 0)
    add_events(calendar, calendar_width, calendar_height)
    calendar.border()
    calendar.refresh()

    # weather window
    weather_width = int (cols* ( 1/ 2))
    weather = newwin(calendar_height, weather_width, clock_height, calendar_width)
    add_weather(weather, weather_width, calendar_height)
    weather.border()
    weather.refresh()

    while True:
        clock.clear()
        time = arrow.utcnow().shift(hours=2).format(format_string)
        clock.addstr(clock_y,clock_x,time)
        clock.refresh()
        clock.border()
        # 1000 = 1 sec 10^3
        clock.timeout(10**3 * 6)
        try: 
            key = clock.getkey()
            if str(key) == 'q':
                break
        except:
            pass



if __name__ == "__main__":
    # Load dot env variables for other modules
    from dotenv import load_dotenv
    import os

    load_dotenv('.env')

    wrapper(main)