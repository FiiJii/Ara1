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


def get_active_coins_symbols(config):
    current_config = BotConfig.objects.get(pk=pk)
    pairs = current_config.currencies.all()
    coins=set();
    for pair in pairs:
        for coin in pair.symbol.split("_"):
            coins.add(coin)
    return coins

def get_active_coins(config):
    symbols=get_active_coins_symbols(config)
    return Coin.objects.filter(symbol__in=symbols);
