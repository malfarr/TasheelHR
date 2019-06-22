<?php
global $base_url;

$name_box = '
  <table style="vertical-align: text-top; border:none; width:100%;" border="0">
    <tr >
      <td style="width: 33%; border:none;" class="text-small text-center">الإسم الأول</td>      
      <td style="width: 33%; border:none;" class="text-small text-center">الأب</td>
      <td style="width: 33%; border:none;" class="text-small text-center">العائلة</td>
    </tr>
    <tr >
      <td style="width: 33%; color: #fff; border: 1px solid #221e1f;" class="">Name</td>
      <td style="width: 33%; color: #fff; border: 1px solid #221e1f;" class="">Middle</td>
      <td style="width: 33%; color: #fff; border: 1px solid #221e1f;" class="">Last</td>
    </tr>
  </table>';

$payment_box = '
  <table style="width: 140px; display: inline; border: medium none; vertical-align: text-top;">
    <tr>      
      <td style="width: 30px; color: #fff;">Decim</td>      
      <td style="width: 60px; color: #fff;">Number4d</td>
      <td style="width: 30px;" class="bold">' . variable_get(MED_VAR_ADMIN_FINANCE_CURRENCY_CODE, 'JOD') . '</td>
    </tr>
  </table>';

$statments[] = '<div class="float-right" style="width: 85px; height: 40px; padding-top: 14px;">' . 'أنا الموقع ادناه ' . '</div>';
$statments[] = '<div class="float-right text-center" style="width: 245px; height: 40px; padding-top:-10px;" dir="rtl">' . $name_box . '</div>';
$statments[] = '<div class="float-right" style="width: 75px;height: 40px; padding-top: 14px;">&nbsp;' . 'وأحمل وثيقة' . '</div>';
$statments[] = '<div class="float-right color-gray" style="width: 138px;height: 40px; padding-top: 14px;">' . ' ____________________ ' . '</div>';
$statments[] = '<div class="float-right" style="width: 40px;height: 40px; padding-top: 14px;">' . 'رقمها' . '</div>';
$statments[] = '<div class="float-right color-gray" style="width: 135px;height: 40px; padding-top: 14px;">' . ' ____________________ ' . '</div>';
$statments[] = '<div class="float-right" style="width: 100px;height: 30px;">' . 'أقر بأنني استلمت ' . '</div>';
$statments[] = '<div class="float-right text-center" style="width: 170px; height: 40px;" dir="rtl">' . $payment_box . '</div>';
$statments[] = '<div class="float-right" style="width: 70px;height: 40px;">&nbsp;' . 'مبلغ وقدره' . '</div>';
$statments[] = '<div class="float-right" style="width: 377px;height: 40px;">' . '<span class="color-gray">_________________________________________________________</span>' . '</div>';
$statments[] = '<div class="float-right" style="width: 250px;height: 30px;">من ' . variable_get(MED_VAR_ADMIN_IMPACT_ORG_ARABIC_NAME, '') . '</div>';
$statments[] = '<div class="float-right" style="width: 60px;height: 30px;">' . 'وذلك بدل' . '</div>';
$statments[] = '<div class="float-right color-gray" style="width: 408px;height: 20px;">' . '_____________________________________________________________' . '</div>';
$statments[] = '<div class="float-right color-gray" style="width: 725px;height: 20px;">' . '____________________________________________________________________________________________________________' . '</div>';
?>

<?php if ($variables['add_title_row']) { ?>
  <div class="col-12">
    <div class="text-center title-row">Payment Voucher<br/> سند صرف</div>
  </div>
<?php } ?>
<div class="col-12">
  <div class="col-4">
    <div style="padding-top: 20px;">Date: <?php echo $variables['date']; ?></div>
    <div>Location: <?php echo $variables['office']; ?></div>
    <?php if ($variables['venue_e_name'] != '' ||
        $variables['venue_a_name'] != '') {
      ?>
      <div>Venue: <?php echo (($variables['venue_e_name'] != '') ? $variables['venue_e_name'] . '<br />' : ''); ?> <?php echo (($variables['venue_a_name'] != '') ? $variables['venue_a_name'] : ''); ?></div>
<?php } ?>
  </div>
  <div class="col-4">
    <table width="95%">      
      <tr>
        <td style="width: 100%;" class="text-center">IMPACT Transaction ID</td>                    
      </tr>
      <tr>
        <td style="width: 100%; height: 88px" class=""></td>                    
      </tr>
    </table>
  </div>
  <div class="col-4">      
    <!--<div class="col-12 text-center">Serial Number</div>-->
    <div class="col-12 text-center padding-5">Serial Number</div>
    <div class="col-12 bold">        
      <table width="100%">
        <tr class="padding-10-0">            
          <td style="" class="bold text-center"><?php echo $variables['sn']; ?></td>            
        </tr>
      </table>
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
<div class="col-12  header-row"><?php echo $variables['title']; ?></div>
<div class="col-12">
  <div dir="rtl" class="extra-line-hight">
    <?php
    foreach ($statments as $statment) {
      echo $statment;
    }
    ?>          
  </div>  
  <br />
  <div class="col-12 box-border padding-5 margin-bottom-5" style="height: 70px">
    Description of Payment: 
  </div>
  <br />  
  <div class="col-4 text-center">
    <div class="margin-bottom-5">Employee</div>
    <div class="text-left text-small" style="border-bottom: 1px solid #CCCCCC; width: 100%; margin-top: 10px;">Name:</div>
    <br />
    <div class="text-right text-small" style="border-bottom: 1px solid #CCCCCC; width: 100%; margin-top: 5px;" dir="rtl">الأسم:</div>    
    <br />
    <div class="text-left text-small" style="border-bottom: 1px solid #CCCCCC; width: 100%; margin-top: 10px;">Signature:</div>    
  </div>
  <div class="col-4 text-center color-white">
    Verified by
  </div>
  <div class="col-4 text-center">
    <div class="col-12">التوقيع</div>
    <br />
    <div class="col-12 color-gray" style="border-bottom: 1px solid #CCCCCC; width: 100%; margin-top: 10px;"></div>
    <br />
    <div class="col-12-right text-right">
      <div class="col-6 text-left">Recipient Phone# </div>
      <div class="col-6 text-right">رقم هاتف المستلم</div>
    </div>
    <div class="col-12-right">
      <div class="col-12 bold">        
        <?php echo variable_get(MED_VAR_ADMIN_FILES_PDF_PHONE_HTML_STRUCTURE, ''); ?>      
      </div>
    </div>    
  </div>
</div>
<hr />
<div class="col-12 text-center">
  <div class="col-6 text-small text-left">Scanned image of recipient ID</div>  
  <div class="col-6 text-small text-right">مرفق صورة إثبات المستلم</div>  
</div>