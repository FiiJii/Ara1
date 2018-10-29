from configuration.models import BotConfig
import requests

def get_config_bot_queryset():
    return BotConfig.objects.all()
    
def can_config_bot():
    return not get_config_bot_queryset().filter(bot_status=True).exists()

def get_price_btc():
    response = requests.get("https://www.okex.com/api/v1/ticker.do?symbol=btc_usdt");
    return response.json()["ticker"]["last"];

def get_ticker_coin(coin):
    response = requests.get("https://www.okex.com/api/v1/ticker.do?symbol="+coin);
    return response.json()["ticker"];