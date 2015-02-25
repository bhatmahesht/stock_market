<?php

/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

class equityTradeDay {

    var $open;
    var $high;
    var $low;
    var $close;
    var $last;
    var $prevclose;
    var $holdings_qty;
    var $isOnline;
    var $doSquareOff;
    var $buyOrders;
    var $daybuyOrders;
    var $sellOrders;
    var $daysellOrders;
    var $daybrokerage;
    var $dayProfit;
    var $buyorderqty;
    var $buyorderdiff;
    var $sellorderqty;
    var $sellorderdiff;
    var $totalbuyqty;
    var $totalsellqty;
    var $totalbuyval;
    var $totalsellval;

    public function __construct() {
        $this->open = 0;
        $this->high = 0;
        $this->low = 0;
        $this->close = 0;
        $this->last = 0;
        $this->prevclose = 0;
        $this->holdings_qty = 0;
        $this->isOnline = FALSE;
        $this->doSquareOff = TRUE;
        $this->daybrokerage = 0;
        $this->dayProfit = 0;
        $this->buyorderqty = 1;
        $this->buyorderdiff = 1;
        $this->sellorderqty = 1;
        $this->sellorderdiff = 1;
        $this->totalbuyqty = 0;
        $this->totalsellqty = 0;
        $this->totalbuyval;
        $this->totalsellval;
        $this->buyOrders = array();
        $this->daybuyOrders = array();
        $this->sellOrders = array();
        $this->daysellOrders = array();
    }

    public function createBuyOrders() {
        $val = $this->prevclose - $this->buyorderdiff;
        while ($val > $this->low) {

            $order['buyval'] = $val;
            $order['buyqty'] = $this->buyorderqty;
            $order['buyamount'] = $order['buyval'] * $order['buyqty'];

            $this->totalbuyqty += $order['buyqty'];
            $this->totalbuyval += $order['buyamount'];
            $this->buyOrders[] = $order;

            $val = $val - $this->buyorderdiff;
        }

        if ($this->totalbuyQty != 0) {
            $this->average_buy = $this->totalBuyVal / $this->totalbuyQty;
        }
    }

    public function createSellOrders() {

        $val = $this->prevclose + $this->sellorderdiff;

        while ($val < $this->high && $this->holdings_qty >= $this->sellorderqty) {

            $order['sellval'] = $val;
            $order['sellqty'] = $this->sellorderqty;
            $order['sellamount'] = $order['sellval'] * $order['sellqty'];

            $this->totalSellQty += $order['sellqty'];
            $this->totalSellVal += $order['sellamount'];

            $this->sellOrders[] = $order;

            $val = $val + $this->sellorderdiff;
        }

        if ($this->totalSellQty != 0) {
            $this->average_sell = $this->totalSellVal / $this->totalSellQty;
        }
    }

    public function squareoffdayorders() {
        if ($this->totalbuyqty >= $this->totalsellqty) {
            $this->squareoffdaysellorders();
        } else {
            $this->squareoffdaybuyorders();
        }
    }

    public function squareoffdaysellorders() {
        $nobuyorders = $this->totalsellqty / $this->buyorderqty;
        $remind = $this->totalsellqty % $this->buyorderqty;
        $daybuyamount = 0;
        $this->daysellOrders = $this->sellOrders;
        $this->sellOrders = array();
        for ($i = 0; $i < $nobuyorders; $i++) {
            $order = array_shift($this->buyOrders);
            $this->daybuyOrders[] = $order;
            $daybuyamount += $order['buyamount'];
        }
        if ($remind != 0) {
            $order = $this->buyOrders[0];
            $this->buyOrders[0]['buyqty'] -=$remind;
            $order['buyqty'] = $remind;
            $order['buyamount'] = $order['buyqty'] * $order['buyval'];
            $daybuyamount += $order['buyamount'];
            $this->daybuyOrders[] = $order;
        }
        if ($this->isOnline) {
            $remainbuyamount = $this->totalbuyval - $daybuyamount;
            $remainqty = $this->totalbuyqty - $this->totalsellqty;
            $averagebuy = $remainbuyamount / $remainqty;
            if ($this->last > $averagebuy) {
                $order['sellqty'] = $remainqty;
                $order['sellval'] = $this->last;
                $order['sellamount'] = $remainqty * $this->last;
                $this->daysellOrders[] = $order;
                $this->totalsellval += $order['sellamount'];
                $daybuyamount = $this->totalbuyval;
            }
        }
        $grossProfit = $this->totalsellval - $daybuyamount;
        $this->daybrokerage = ($this->totalsellval + $daybuyamount) * 0.0005;
        $this->dayProfit = $grossProfit - $this->daybrokerage;
    }

    public function squareoffdaybuyorders() {
        $nosellorders = $this->totalbuyqty / $this->sellorderqty;
        $remind = $this->totalbuyqty % $this->sellorderqty;
        $daysellamount = 0;
        for ($i = 0; $i < $nosellorders; $i++) {
            $order = array_shift($this->sellOrders);
            $this->daysellOrders[] = $order;
            $daysellamount += $order['sellamount'];
        }
        if ($remind != 0) {
            $order = $this->sellOrders[0];
            $this->sellOrders[0]['sellqty'] -= $remind;
            $order['buyqty'] = $remind;
            $order['sellamount'] = $order['sellqty'] * $order['sellval'];
            $daysellamount +=$order['sellamount'];
            $this->daysellOrders[] = $order;
        }

        if ($this->isOnline) {
            $remainsellamount = $this->totalsellval - $daysellamount;
            $remainqty = $this->totalsellqty - $this->totalbuyqty;
            $averagesell = $remainsellamount / $remainqty;
            if ($this->last < $averagesell) {
                $order['buyqty'] = $remainqty;
                $order['buyval'] = $this->last;
                $order['buyamount'] = $remainqty * $this->last;
                $this->daybuyOrders[] = $order;
                $this->totalbuyval += $order['buyamount'];
                $daysellamount = $this->totalsellval;
            }
        }
        $grossProfit = $daysellamount - $this->totalbuyval;
        $this->daybrokerage = ($daysellamount + $this->totalbuyval) * 0.0005;
        $this->dayProfit = $grossProfit - $this->daybrokerage;
    }

    public function getdayValues($open, $high, $low, $close, $last, $prevclose) {
        $this->open = $open;
        $this->high = $high;
        $this->low = $low;
        $this->close = $close;
        $this->last = $last;
        $this->prevclose = $prevclose;
    }
    
    public function executeOrders(){
        
    }

}
