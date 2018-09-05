from django.db import models

# Create your models here.
class Transaction(models.Model):
    creation_date = models.DateTimeField(auto_now_add=True)
    investment = models.DecimalField(max_digits=19, decimal_places=10) 
    earnings   = models.DecimalField(max_digits=19, decimal_places=10)

class TransactionDetail(models.Model):
    transaction = models.ForeignKey(Transaction, null=True, blank=True, on_delete=models.CASCADE, related_name='details')
    parity = models.CharField(max_length=100)
    amount = models.DecimalField(max_digits=19, decimal_places=10)
    commission = models.DecimalField(max_digits=19, decimal_places=10)
    okex_order = models.IntegerField(default=0)
    transaction_order = models.IntegerField(default=0)