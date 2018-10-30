from django.db import models

VERBOSITY_CHOICES = (
    ("silent","silent"),
    ("medium","medium"),
)

STATUS_CHOICES = (
    ("active","active"),
    ("inactive","inactive"),
)

class Currency(models.Model):
    name = models.CharField(max_length=100)
    symbol = models.CharField(max_length=100)
    name_symbol = models.CharField(max_length=100)
    description = models.CharField(max_length=150)
    status = models.CharField(max_length=100, choices=STATUS_CHOICES)

class Coin(models.Model):
    symbol = models.CharField(max_length=100)
    name = models.CharField(max_length=100)

class BotConfig(models.Model):
    bot_status = models.BooleanField(default=False)
    time_interval = models.IntegerField(default=0)
    max_lost = models.DecimalField(max_digits=19, decimal_places=10)
    db_verbosity = models.CharField(max_length=100, choices=VERBOSITY_CHOICES)
    currencies = models.ManyToManyField(Currency, related_name='configs')

class Exchange(models.Model):
    name = models.CharField(max_length=100)
    status = models.BooleanField(default=False)




