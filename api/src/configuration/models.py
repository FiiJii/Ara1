from django.db import models

VERBOSITY_CHOICES = (
    ("silent","Silent"),
    ("medium","Medium"),
)

class Currency(models.Model):
    name = models.CharField(max_length=100)
    symbol = models.CharField(max_length=100)
    description = models.CharField(max_length=150)

class BotConfig(models.Model):
    bot_status = models.BooleanField(default=False)
    time_interval = models.IntegerField(default=0)
    max_lost = models.DecimalField(max_digits=19, decimal_places=10)
    db_verbosity = models.CharField(max_length=100, choices=VERBOSITY_CHOICES)
    currencies = models.ManyToManyField(Currency, related_name='configs')

class Exchange(models.Model):
    name = models.CharField(max_length=100)
    status = models.BooleanField(default=False)




