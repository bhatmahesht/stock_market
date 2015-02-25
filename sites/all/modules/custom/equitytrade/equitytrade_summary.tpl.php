<?php
/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
?>
<table>
    <thead>
        <tr>
            <th>Property</th>
            <th>Value</th>
        </tr>
    </thead>
    <tr>
        <td> Total Buy Qty </td>
        <td> <?php print $equitytrade->totalbuyQty ?> </td>
    </tr>
    <tr> 
        <td> Total Buy Value </td>
        <td> <?php print $equitytrade->totalBuyVal ?> </td>
    </tr>
    <tr>
        <td> Average Buy Value</td>
        <td> <?php print $equitytrade->average_buy ?> </td>
    </tr>
    <tr>
        <td> Total Sell Qty </td>
        <td> <?php print $equitytrade->totalSellQty ?> </td>
    </tr>
    <tr>
        <td> Total Sell Value </td>
        <td> <?php print $equitytrade->totalSellVal ?> </td>
    </tr>
    <tr>
        <td> Average Sell value </td>
        <td> <?php print $equitytrade->average_sell ?> </td>
    </tr>
    <tr> 
        <td> Gross Profit </td>
        <td> <?php print $equitytrade->grossProfit ?> </td>
    </tr>
    <tr>
        <td> Brokerage </td>
        <td> <?php print $equitytrade->brokerage ?> </td>
    </tr>
    <tr>
        <td> Net Profit </td>
        <td> <?php print $equitytrade->netProfit ?> </td>
    </tr>
    <tr>
        <td> Net holdings</td>
        <td> <?php print $equitytrade->holdings_qty ?> </td>
    </tr>
    <tr>
        <td> Net holdings value </td>
        <td> <?php print $equitytrade->holdings_qty * $equitytrade->average_buy ?> </td>
    </tr>
    <tr>
        <td> Amount needed </td>
        <td> <?php print $equitytrade->amount_needed ?> </td>
    </tr>
    <tr>
        <td> Max Amount needed </td>
        <td> <?php print $equitytrade->max_amount_needed ?> </td>
    </tr>

</table>