<script>
  (function ($, Drupal, window, document, undefined) {
    $(document).ready(function () {
      users_list = $('.users-list-datatable').dataTable({
        aaSorting: [],
        columnDefs: [{orderable: false, targets: 'nosort'}, {searchable: false, targets: 'nosearch'}, {targets: 'colspan-enabled', defaultContent: ''}],
        pageLength: 10,
        lengthMenu: [[10, 20, 30, -1], [10, 20, 30, 'All']],
        buttons: ['copy', 'csv', 'excel', 'print'],
      }).api();

      users_list.on('order.dt search.dt', function () {
        users_list.column(0, {search: 'applied', order: 'applied'}).nodes().each(function (cell, i) {
          cell.innerHTML = i + 1;
        });
      }).draw();

      $.fn.dataTableExt.afnFiltering.push(function (oSettings, aData, iDataIndex) {
        if (show_blocked_users) {
          return true;
        } else {
          return aData[10] == 'Active';
        }
      });

      show_blocked_users = false;
      $(".show-blocked-users").on('change', function () {
        // $radio = $("input[name='RadioGroup1']:checked").val();
        show_blocked_users = $(this).is(":checked");
        users_list.draw(); // refresh the dataTable
      });
      users_list.draw();
    });
  })(jQuery, Drupal, this, this.document);
</script>

<div id="page-users-wrapper" class="page-wrapper page-users-wrapper">
  <div id="users-list" class="block">
    <div class="block-title">
      <h2><strong>Users</strong> list</h2>
      <div class="block-options">
        <a href="javascript:void(0)" class="btn btn-alt btn-sm btn-primary" data-toggle="block-toggle-content"><?php print MED_FA_ARROWS_V; ?></a>
        <a href="javascript:void(0)" class="btn btn-alt btn-sm btn-primary" data-toggle="block-toggle-fullscreen"><?php print MED_FA_EXPAND; ?></a>
      </div>
    </div>
    <div class="block-content">      
      <div class="table-options clearfix">
        <div class="btn-group btn-group-sm pull-left"> 
          <div class="form-type-checkbox form-item-refresher-workshop form-item checkbox form-group">
            <input type="checkbox" class="show-blocked-users" id="show-blocked-users" name="show-blocked-users"><label for="show-blocked-users"><strong>Show blocked accounts</strong></label>
          </div>
        </div>
        <?php if (isset($variables['users_plan_msg']) && !empty($variables['users_plan_msg'])) { ?>
          <div class="pull-right text-uppercase user-plan-message"> 
            <?php echo check_markup($variables['users_plan_msg'], 'full_html'); ?>
          </div>
        <?php } ?>
      </div>     
      <div class="table-responsive">
        <?php print theme('table', $variables['list']); ?>
      </div>
    </div>
  </div>
</div>