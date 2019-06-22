<?php
global $base_url;

$payment_box = '
  <table style="width: 140px; display: inline; border: medium none; vertical-align: text-top;">
    <tr>      
      <td style="width: 30px;" class="bold">' . variable_get(MED_VAR_ADMIN_FINANCE_CURRENCY_CODE, 'JOD') . '</td>
      <td style="width: 60px; color: #fff;">Number4d</td>
      <td style="width: 30px; color: #fff;">Decim</td>
    </tr>
  </table>';

$vendor_box = '
  <table style="width: 100%; display: inline; vertical-align: text-top; border: none;">
    <tr>      
      <td style="border: none;" class="bold">Vendor:</td>
      <td style="border: none">___________________________________________________</td>
      <td style="border: none; text-align: right;">___________________________________________________</td>
      <td style="border: none; text-align: right;" class="bold">المورد:</td>      
    </tr>
  </table>';
$invoice_box = '
  <table style="width: 100%; display: inline; vertical-align: text-top; border: none; margin-top: 10px;">
    <tr>      
      <td style="border: none;" class="bold">No:</td>
      <td style="border: 1px solid; color: #fff;" class="bold">DD</td>
      <td style="border: 1px solid; color: #fff;" class="bold">DD</td>
      <td style="border: 1px solid; color: #fff;" class="bold">DD</td>
      <td style="border: 1px solid; color: #fff;" class="bold">DD</td>
      <td style="border: 1px solid; color: #fff;" class="bold">DD</td>
      <td style="border: 1px solid; color: #fff;" class="bold">DD</td>
      <td style="border: 1px solid; color: #fff;" class="bold">DD</td>
      <td style="border: 1px solid; color: #fff;" class="bold">DD</td>
      <td style="border: 1px solid; color: #fff;" class="bold">DD</td>
      <td style="border: 1px solid; color: #fff;" class="bold">DD</td>      
      <td style="border: none; text-align: right;" class="bold">Date:</td>  
      <td style="border: 1px solid; color: #fff;" class="bold">DD</td>
      <td style="border: 1px solid; color: #fff;" class="bold">DD</td>
      <td style="border: none; text-align: right;" class="bold"> - </td>
      <td style="border: 1px solid; color: #fff;" class="bold">DD</td>
      <td style="border: 1px solid; color: #fff;" class="bold">DD</td>
      <td style="border: none; text-align: right;" class="bold"> - </td>
      <td style="border: 1px solid; color: #fff;" class="bold">DD</td>
      <td style="border: 1px solid; color: #fff;" class="bold">DD</td>
      <td style="border: 1px solid; color: #fff;" class="bold">DD</td>
      <td style="border: 1px solid; color: #fff;" class="bold">DD</td>
    </tr>
  </table>';
?>

<div class="col-12">
  <div class="col-4">
    <div style="padding-top: 20px;">Date: </div>
    <div>Location: </div>
  </div>
  <div class="col-4">
    <table width="95%">      
      <tr>
        <td style="width: 100%;" class="text-center">Transaction ID</td>                    
      </tr>
      <tr>
        <td style="width: 100%; height: 88px" class=""></td>                    
      </tr>
    </table>
  </div>
  <div class="col-4">      
    <div class="col-12 text-right">
      <h2 style="margin: 0 0 29px 0; padding: 0;">@[month] - @[year]</h2>
    </div>
    <div class="col-12 text-center padding-5">Value in English Numerals</div>
    <div class="col-12 bold">
      <table width="100%">
        <tr>
          <td class="text-center bold" style="width: 20%;"><?php echo variable_get(MED_VAR_ADMIN_FINANCE_CURRENCY_CODE, 'JOD'); ?></td>
          <td style="width: 50%;"></td>
          <td style="width: 30%;"></td>
        </tr>
      </table>      
    </div>
  </div>
</div>
<br />
<div class="col-12 box-border padding-5 margin-bottom-5" style="height: 90px">
  Description: 
</div>
<div class="col-12 "><?php echo $vendor_box; ?></div>
<div class="col-12 "><?php echo $invoice_box; ?></div>
<hr />
<div class="col-12 text-center">
  <div class="col-6 text-small text-left">Scanned image of receipt</div>  
  <div class="col-6 text-small text-right">مرفق صورة الفاتورة</div>  
</div>