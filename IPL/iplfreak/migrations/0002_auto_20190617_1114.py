# Generated by Django 2.2.1 on 2019-06-17 05:44

from django.db import migrations, models


class Migration(migrations.Migration):

    dependencies = [
        ('iplfreak', '0001_initial'),
    ]

    operations = [
        migrations.AlterField(
            model_name='match',
            name='city',
            field=models.CharField(blank=True, max_length=64, null=True),
        ),
        migrations.AlterField(
            model_name='match',
            name='toss_decision',
            field=models.IntegerField(),
        ),
    ]
