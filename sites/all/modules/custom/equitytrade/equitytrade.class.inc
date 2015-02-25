<?php

/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

class equityTrade {

    var $initial_amount;
    var $holdings_qty;
    var $holdings_value;
    var $average_buy;
    var $average_sell;
    var $current_amount;
    var $brokerage;
    var $buyorderqty;
    var $buyorderdiff;
    var $sellorderqty;
    var $sellorderdiff;
    var $amount_needed;
    var $max_amount_needed;
    var $buyOrders;
    var $sellOrders;
    var $totalbuyQty;
    var $totalBuyVal;
    var $totalSellQty;
    var $totalSellVal;
    var $grossProfit;
    var $netProfit;

    public function __construct() {
        $this->initial_amount = 0;
        $this->holdings_qty = 0;
        $this->holdings_value = 0;
        $this->brokerage = 0;
        $this->amount_needed = 0;
        $this->max_amount_needed = 0;
        $this->totalbuyQty = 0;
        $this->totalBuyVal = 0;
        $this->totalSellQty = 0;
        $this->totalSellVal = 0;
        $this->buyorderdiff = 1;
        $this->buyorderqty = 1;
        $this->sellorderqty = 1;
        $this->sellorderdiff = 1;
        $this->grossProfit = 0;
        $this->netProfit = 0;
        $this->buyOrders = array();
        $this->sellOrders = array();
    }

    public function createBuyOrders($prevclose, $low, $bhavdate) {

        $buyOrders = array();
        $val = $prevclose - $this->buyorderdiff;
        while ($val > $low) {

            $order['buyval'] = $val;
            $order['buyqty'] = $this->buyorderqty;
            $order['buyamount'] = $order['buyval'] * $order['buyqty'];
            $this->current_amount -= $order['buyamount'];
            $this->amount_needed += $order['buyamount'];
            $this->holdings_qty += $order['buyqty'];
            $this->totalbuyQty += $order['buyqty'];
            $this->totalBuyVal += $order['buyamount'];
            $this->brokerage += $order['buyamount'] * 0.005;
            $buyOrders[] = $order;

            $val = $val - $this->buyorderdiff;
        }
        if ($this->totalbuyQty != 0) {
            $this->average_buy = $this->totalBuyVal / $this->totalbuyQty;
        }
        $this->amount_needed = $this->amount_needed + $this->brokerage;
        if ($this->amount_needed > $this->max_amount_needed) {
            $this->max_amount_needed = $this->amount_needed;
        }
        $dt = date('d m Y', $bhavdate);
        $this->buyOrders["$dt"] = $buyOrders;
    }

    public function createSellOrders($prevclose, $high, $bhavdate) {

        $sellOrders = array();
        $val = $prevclose + $this->sellorderdiff;

        while ($val < $high && $this->holdings_qty >= $this->sellorderqty) {

            $order['sellval'] = $val;
            $order['sellqty'] = $this->sellorderqty;
            $order['sellamount'] = $order['sellval'] * $order['sellqty'];

            $this->current_amount += $order['sellamount'];
            $this->holdings_qty -= $order['sellqty'];
            $this->amount_needed -= $order['sellamount'];
            $this->totalSellQty += $order['sellqty'];
            $this->totalSellVal += $order['sellamount'];

            $this->brokerage += $order['sellamount'] * 0.005;
            $sellOrders [] = $order;

            $val = $val + $this->sellorderdiff;
        }
        if ($this->totalSellQty != 0) {
            $this->average_sell = $this->totalSellVal / $this->totalSellQty;
        }

        $this->amount_needed = $this->amount_needed + $this->brokerage;

        $this->grossProfit = $this->totalSellVal - $this->totalSellQty * $this->average_buy;
        $this->netProfit = $this->grossProfit - $this->brokerage;

        $dt = date('d m Y', $bhavdate);
        $this->sellOrders["$dt"] = $sellOrders;
    }

    public function executeTrade($bhavdate, $open, $high, $low, $close, $last, $prevclose) {
        $trday = new equityTradeDay();

        $trday->getdayValues($open, $high, $low, $close, $last, $prevclose);
        $trday->buyorderqty = $this->buyorderqty;
        $trday->buyorderdiff = $this->buyorderdiff;
        $trday->sellorderqty = $this->sellorderqty;
        $trday->sellorderdiff = $this->sellorderdiff;

        $trday->createBuyOrders();
        $trday->createSellOrders();

        if ($this->isDayTrade) {
            $trday->squareoffdayorders();
        }

        $this->buyOrders["$bhavdate"] = $trday->buyOrders;
        $this->sellOrders["$bhavdate"] = $trday->sellOrders;

        foreach ($trday->buyOrders as $order) {
            $this->totalbuyQty += $order['buyqty'];
            $this->holdings_qty += $order['buyqty'];
            $this->totalBuyVal += $order['buyamount'];
            $this->brokerage += $order['buyamount'] * 0.005;
            $this->amount_needed += ($order['buyamount'] + $order['buyamount'] * 0.005);
            $this->current_amount -= $order['buyamount'];
        }

        if ($this->totalbuyQty != 0) {
            $this->average_buy = $this->totalBuyVal / $this->totalbuyQty;
        }

        if ($this->amount_needed > $this->max_amount_needed) {
            $this->max_amount_needed = $this->amount_needed;
        }

        foreach ($trday->sellOrders as $order) {
            $this->totalSellQty += $order['sellqty'];
            $this->holdings_qty -= $order['sellqty'];
            $this->totalSellVal += $order['sellamount'];
            $this->brokerage += $order['sellamount'] * 0.005;
            $this->amount_needed -= ($order['sellamount'] - $order['sellamount'] * 0.005);
        }

        if ($this->totalSellQty != 0) {
            $this->average_sell = $this->totalSellVal / $this->totalSellQty;
        }

        $this->grossProfit = $this->totalSellVal - $this->totalSellQty * $this->average_buy;
        $this->netProfit = $this->grossProfit - $this->brokerage;
    }

}
