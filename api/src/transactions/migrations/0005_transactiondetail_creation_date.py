# Generated by Django 2.1 on 2018-10-18 16:57

import datetime
from django.db import migrations, models


class Migration(migrations.Migration):

    dependencies = [
        ('transactions', '0004_transactiondetail_action'),
    ]

    operations = [
        migrations.AddField(
            model_name='transactiondetail',
            name='creation_date',
            field=models.DateTimeField(auto_now_add=True, default=datetime.datetime(2018, 10, 18, 16, 57, 37, 541402)),
            preserve_default=False,
        ),
    ]