from bs4 import BeautifulSoup
from types import SimpleNamespace
from os import getenv
from requests import get



def parse_rows(rows):
    result = []
    for row in rows[1:]:
        obj = SimpleNamespace()
        obj.hour = row.find('th', class_='hourly-table-hour').span.text
        obj.temp = row.find('td', class_='temp').span.text
        obj.feels = row.find('td', class_='hourly-table-feelslike').get('title')
        obj.cloud = row.find('img', class_='wxImage').get('title')
        obj.cloudImageUrl = row.find('img', class_='wxImage').get('src')
        obj.wind = row.find('td', class_='wind').div.get('title')
        result.append(obj)

    return result

def get_weather():
    # Example url https://www.ilmatieteenlaitos.fi/saa/helsinki
    url = getenv('WEATHER_URL')
    page = get(url)
    soup = BeautifulSoup(page.content, 'html.parser')

    # Find all tr:s with id hour-row-*
    rows = soup.findAll("tr", {"id" : lambda L: L and L.startswith('hour-row-')})

    return parse_rows(rows)

if __name__ == "__main__":
    print(get_weather())